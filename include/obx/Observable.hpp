#ifndef __OBSERVABLE_HPP__
#define __OBSERVABLE_HPP__

#include "fwd_decl.h"

#include "State.h"

#include "log.h"

#include <unordered_set>

namespace obx {

namespace {
	template<class, class = std::void_t<>>
	struct has_equal_operator : std::false_type { };

	template<class T>
	struct has_equal_operator<T, std::void_t<decltype(std::declval<T>() == std::declval<T>())>> : std::true_type { };

	template<class T>
	inline constexpr bool has_equal_operator_v = has_equal_operator<T>::value;
}

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
			checkWrite();
			IObservable::invalidateObservers();
			return val;
		}
		T* operator->() { return std::pointer_traits<T*>::pointer_to(*(*this)); }

		template<class Arg>
		Observable& operator=(Arg&& arg) {
			checkWrite();

			if constexpr(std::is_move_constructible_v<T> && has_equal_operator_v<T>) {
				const T tmp(std::move(val));
				val = std::forward<Arg>(arg);
				if(!(tmp == val)) {
					IObservable::invalidateObservers();
				}
			}
			else {
				val = std::forward<Arg>(arg);
				IObservable::invalidateObservers();
			}
			return *this;
		}

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

		void checkWrite() const {
			log<LogLevel::DEBUG>("\t\tWrite\n");
			if(obx::state.isInAction() == false) {
				log<LogLevel::EXCEPTION>("Mutating observable outside an action");
			}
		}
};

}

#endif
