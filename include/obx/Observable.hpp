#ifndef __OBSERVABLE_HPP__
#define __OBSERVABLE_HPP__

#include "fwd_decl.h"

#include "State.h"

#include "log.h"

#include <unordered_set>

namespace obx {

class IObservable {
	public:
		IObservable(const IObservable&) = delete;
		IObservable(IObservable&&) = delete;

		bool isBeingObserved() const;
		void markAsObserved() const;
		void invalidateObservers() const;

	protected:
		mutable std::unordered_set<const Observer*> observers;

		IObservable();
		void addObserver(const Observer* observer) const;
		void removeObserver(const Observer* observer) const;

		friend class obx::Observer;
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
			log<LogLevel::DEBUG>("\t\tWrite\n");
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
			log<LogLevel::DEBUG>("\t\tRead Observable\n");
			IObservable::markAsObserved();
			return val;
		}
		operator const T&() const { return *(*this); }
		const T& operator()() const { return *this; }
		const T* operator->() const { return std::pointer_traits<T*>::pointer_to(*this); }

	private:
		T val;
};

}

#endif
