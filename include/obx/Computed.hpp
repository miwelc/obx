#pragma once

#include "fwd_decl.h"

#include <iostream>

namespace obx {

template<class T>
class Computed : public Observer, protected IObservable {
	public:
		Computed(const std::function<T(void)>& f)
			: Observer([this, f]() { val = f(); }) { }
		Computed(std::function<T(void)>&& f)
			: Observer([this, f = std::move(f)]() { val = f(); }) { }
		Computed(Computed&&) = default;
		Computed(const Computed&) = delete;

		virtual void markAsTainted() const override {
			Observer::markAsTainted();
			IObservable::invalidateObservers();
		}

		const T& operator()() {
			std::cout << "\t\tRead Computed" << std::endl;
			if(Observer::tainted) {
				std::cout << "\t\tComputing" << std::endl;
				Observer::observe();
			}
			IObservable::markAsObserved();
			return val;
		}
		operator const T&() { return this->operator()(); }

	private:
		T val;
};

}
