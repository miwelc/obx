#ifndef __COMPUTED_H__
#define __COMPUTED_H__

#include "fwd_decl.h"

#include "log.h"

namespace obx {

template<class T>
class Computed : protected Observer, protected IObservable {
	using LogLevel = __::LogLevel;

	public:
		Computed(const std::function<T(void)>& f)
			: Observer([this]() { val = Computed::f(); }), f(f) { }
		Computed(std::function<T(void)>&& f)
			: Observer([this]() { val = Computed::f(); }), f(std::move(f)) { }
		Computed(Computed&& other)
			: Observer(std::move(other)), f(std::move(other.f)), val(other.isTainted() ? T() : std::move(other.val))
		{
			Observer::f = [this]() { val = Computed::f(); };
		}
		Computed(const Computed&) = delete;

		virtual ~Computed() = default;

		const T& operator()() {
			__::log<LogLevel::DEBUG>("\t\tRead Computed\n");
			if(Observer::tainted) {
				__::log<LogLevel::DEBUG>("\t\tComputing\n");
				Observer::observe();
			}
			IObservable::markAsObserved();
			return val;
		}
		operator const T&() { return this->operator()(); }

	protected:
		virtual void markAsTainted() const override {
			Observer::markAsTainted();
			IObservable::invalidateObservers();
		}

	private:
		std::function<T(void)> f;
		T val;
};

}

#endif
