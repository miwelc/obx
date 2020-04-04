#pragma once

#include <utility>
#include <functional>
#include <unordered_map>
#include <unordered_set>

#include <string>
#include <iostream>

class obx {
	private:
		class State;
		class ObserverCanary;
		class ActionCanary;

	public:
		class Observer;

		static void runInAction(const std::function<void(void)>& f) {
			ActionCanary actionCanary;
			f();
		}

		class IObservable {
			public:
				void markAsObserved() const {
					if(obx::state.isInObserver()) {
						obx::state.markAsObserved(this);
					}
				}

				void invalidateObservers() const {
					for(auto observerPtr : observers) {
						observerPtr->markAsTainted();
					}
				}

			protected:
				IObservable() { }
				mutable std::unordered_set<Observer*> observers;

				friend class obx::State;
				friend class obx::Observer;
				friend class obx::ObserverCanary;
		};

		template<class T>
		class Observable : IObservable {
			public:
				template<class... Args>
				Observable(Args&&... args) : val(std::forward<Args>(args)...) { }
				Observable(const T& arg) : val(arg) { }
				Observable(T&& arg) : val(std::move(arg)) { }

				// Write
				T& operator*() {
					std::cout << "\t\tWrite" << std::endl;
					if(obx::state.isInAction() == false) {
						log<LogLevel::EXCEPTION>("Mutating observable outside an action");
					}

					IObservable::invalidateObservers();

					return val;
				}
				template<class Arg>
				Observable& operator=(Arg&& arg) { *(*this) = std::forward<Arg>(arg); return *this; }
				T* operator->() { return std::pointer_traits<T*>::pointer_to(*(*this)); }

				// Read
				const T& operator*() const {
					std::cout << "\t\tRead Observable" << std::endl;
					IObservable::markAsObserved();
					return val;
				}
				operator const T&() const { return *(*this); }
				const T& operator()() const { return *this; }
				const T* operator->() const { return std::pointer_traits<T*>::pointer_to(*this); }

			private:
				T val;
				friend class obx;
		};

		class Observer {
			public:
				Observer(const std::function<void(void)>& f) : f(f) { }
				Observer(std::function<void(void)>&& f) : f(std::move(f)) { }
				Observer(Observer&&) = default;
				Observer(const Observer&) = delete;

				~Observer() { stopObserving(); }

				void observe() {
					ObserverCanary observerCanary(*this);
					f();
					tainted = false;
				}

				void stopObserving() {
					const auto end = observables.end();
					for(auto it = observables.begin(); it != end; it = observables.erase(it)) {
						(*it)->observers.erase(this);
					}
				}

				bool isTainted() const {
					return tainted;
				}

				virtual void markAsTainted() {
					tainted = true;
				}

			protected:
				const std::function<void(void)> f;
				mutable std::unordered_set<const IObservable*> observables;
				bool tainted = true;

				friend class obx::State;
				friend class obx::ObserverCanary;
		};

		template<class T>
		class Computed : public Observer, protected IObservable {
			public:
				Computed(const std::function<T(void)>& f)
					: Observer([this, f]() { observable.val = f(); }) { }
				Computed(std::function<T(void)>&& f)
					: Observer([this, f = std::move(f)]() { observable.val = f(); }) { }
				Computed(Computed&&) = default;
				Computed(const Computed&) = delete;

				virtual void markAsTainted() override {
					Observer::markAsTainted();
					IObservable::invalidateObservers();
				}

				const T& operator()() {
					if(Observer::tainted) {
						std::cout << "\t\tComputing" << std::endl;
						Observer::observe();
					}
					std::cout << "\t\tRead Computed" << std::endl;
					IObservable::markAsObserved();
					return observable.val;
				}
				operator const T&() { return this->operator()(); }

			private:
				Observable<T> observable{};
		};

	private:
		class Canary;
		class ActionCanary;
		class ObserverCanary;
		class State {
			public:
				State() { }

				bool isInObserver() const { return mode == Mode::OBSERVER; }
				bool isInAction() const { return mode == Mode::ACTION; }
				void markAsObserved(const IObservable* observable) {
					observable->observers.insert(observerPtr);
					observerPtr->observables.insert(observable);
				}

			private:
				enum class Mode : uint32_t {
					OUTSIDE = 0,
					OBSERVER,
					ACTION
				};

				Mode mode = Mode::OUTSIDE;
				Observer* observerPtr = nullptr;

				friend class obx::Canary;
				friend class obx::ActionCanary;
				friend class obx::ObserverCanary;
		};

		inline static State state{};

		obx() { }


		class Canary {
			protected:
				Canary() : oldStateMode(obx::state.mode) { }
				~Canary() { obx::state.mode = oldStateMode; }
			private:
				State::Mode oldStateMode;
		};

		class ActionCanary : Canary {
			public:
				ActionCanary() {
					if(obx::state.isInObserver()) {
						log<LogLevel::EXCEPTION>("Cannot run action inside an observer");
					}
					obx::state.mode = State::Mode::ACTION;
					ActionCanary::nestingLevels++;
				}
				~ActionCanary() {
					if(--ActionCanary::nestingLevels == 0) {
						// RUN REACTIONS
					}
				}

			private:
				inline static uint32_t nestingLevels = 0;
				inline static std::unordered_set<Observer*> invalidatedObservers;
		};

		class ObserverCanary : Canary {
			public:
				ObserverCanary(Observer& observer)
					: oldObserverPtr(obx::state.observerPtr)
				{
					obx::state.mode = State::Mode::OBSERVER;
					obx::state.observerPtr = &observer;

					observer.stopObserving();
				}

				~ObserverCanary() { obx::state.observerPtr = oldObserverPtr; }

			private:
				Observer* oldObserverPtr;
		};



		enum class LogLevel : uint32_t {
			DEBUG = 0,
			INFO,
			ERROR,
			EXCEPTION
		};

		#ifndef GLOBAL_LOG_LEVEL
			#define GLOBAL_LOG_LEVEL DEBUG
		#endif

		static const LogLevel globalLogLevel = LogLevel::GLOBAL_LOG_LEVEL;

		template<LogLevel logLevel>
		static void log(const std::string& message) {
			if constexpr(logLevel < globalLogLevel) {
				return;
			}

			if constexpr(logLevel == LogLevel::DEBUG || logLevel == LogLevel::INFO) {
				std::cout << message << "\n";
			}
			else if constexpr(logLevel == LogLevel::ERROR) {
				std::cerr << message << std::endl;
			}
			else if constexpr(logLevel == LogLevel::EXCEPTION) {
				#if __cpp_exceptions
					throw std::runtime_error(message);
				#else
					std::cerr << message << std::endl;
					std::terminate();
				#endif
			}
		}
};
