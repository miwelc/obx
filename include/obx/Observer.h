#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include "fwd_decl.h"

#include <functional>
#include <unordered_set>

namespace obx {

class Observer {
	public:
		Observer(const std::function<void(void)>& f);
		Observer(std::function<void(void)>&& f);
		Observer(Observer&& other);
		Observer(const Observer&) = delete;

		virtual ~Observer();

		void observe() const;
		void stopObserving() const;

		bool isTainted() const;
		virtual void markAsTainted() const;

	protected:
		std::function<void(void)> f;
		mutable std::unordered_set<const IObservable*> observables;
		mutable bool tainted = true;

		void addObservable(const IObservable* observable) const;
		void removeObservable(const IObservable* observable) const;

	friend class obx::IObservable;
};

}

#endif
