# obx

**obx** is a C++17 reactive programming library heavily inspired by [MobX JS](https://mobx.js.org/README.html). **obx** borrows its *observer/observable* pattern with derived states (*computed values*). In essence:

  - *Actions* modify the state (*observables*).
  - These modified *observables* trigger changes in other derived *observables* (*computed values*).
  - *Observers* that use a modified *observable* get notified.

This library may be used as header-only by just including `obx/obx.h`. Or compiled by:
```sh
cd include/obx
make clean && make
```
This will produce a `libobx.a` that can be statically linked to your application.
When using the library this way, don't forget to globally define `OBX_AS_LIB`. You can do that as a compiler flag `-D OBX_AS_LIB` or adding a `#define OBX_AS_LIB` on top of `obx/obx.h`. Now you can remove `include/obx/impl`.

### Observables
*Observables* wrap some state that may be *observed* (tracked for changes).

  - You can declare them using `obx::Observable<T>`:
    ```c++
    obx::Observable<std::string> obsName = "Thomas";
    obx::Observable<int> obsAge = 25;
    ```
  - Or taking advantage of c++17 class template argument deduction:
    ```c++
    obx::Observable obsName = "Thomas"s;
    obx::Observable obsAge = 25;
    ```

You can access its stored state by:
  - Using its implicit cast operator:
    ```c++
    std::cout << "Birth year: " << (currentYear - obsAge) << std::endl;
    ```
  - If it's not possible to use the implicit conversion in an expression, you can use the `()` operator to access its vaue:
    ```c++
    std::cout << "Name: " << obsName() << std::endl;
    ```

### Computed values
*Computed values* are functions that use *observables* to produce a derived state and keep it cached until an observed *observable* changes. They are declared using `obx::Computed<T>`.

  - Any stateless function returning a value may be used.
    ```c++
    obx::Computed<int> birthYear([&]() {
        return currentYear - obsAge;
    });
    ```
  - You can then use it the same way as normal *observables*:
    ```c++
    std::cout << "Birth year: " << birthYear << std::endl;
    ```
  - If its stored state is not tainted, subsequent uses won't execute again the function.
  - If it gets tainted, the function will be executed lazily when the *computed* gets accessed again.
  - *Computed values* are **read-only**.

### Actions
*Observables* may only be modified by *actions*. You can create an action by:

  - Using a convenient RAII-style mechanism: `obx::Action`
    ```c++
    void setPersonalInfo(const std::string& name, int age) {
        obx::Action _;
        obsName = name;
        obsAge = age;
    }
    ```
  - Inline using `obx::runInAction()`
    ```c++
    obx::runInAction([&]() {
        obsName = "Rick"s;
        obsAge = 19;
    });
    ```

If an *observable* type has a defined T == T operator and is move constructible, then only changes that evaluates preVal == postVal to false get notified to its *observers*. In essence, if you assign the same value to an observable, there won't be any side-effects.

### Derived state flow
If an *action* changes an *observable*, accessing any *computed* that uses that observable will compute a new derived value and cache it for future accesses. Even if a *computed* gets invalidated, no derived value gets recomputed until it's actually used.

```c++
obx::Observable n = 1;
obx::Computed<int> derived1([&]() {
    return n * 2;
});
obx::Computed<float> derived2([&]() {
    return derived1 * 1.5;
});

std::cout << derived2 << std::endl; // Output: 3.0

obx::runInAction([&]() {
    n = 2;
});

std::cout << derived2 << std::endl; // Output: 6.0
```

### Autorun
Some times you may want to automatically detect changes in the state. You can do that using `obx::Autorun`.

```c++
obx::Observable n = 1;
obx::Autorun a([&]() {
    std::cout << "N: " << n << std::endl;
});
// Output: "N: 1"
obx::runInAction([&]() {
    n = 2;
});
// Output: "N: 2"
```

You can disable/enable an *autorun* by using its `disable()` and `enable()` methods.
If you want to declare an *autorun* but enable it later on, you can do so: `obx::Autorun a(observerCB, false)`.

### Reaction
Some times you may want to react to some changes in the state but without tracking all the *observables* you use in your function. You can do that using `obx::Reaction`.

```c++
obx::Observable a = 1;
obx::Observable b = 1;
obx::Reaction r([&]() {
    // Observables here will get tracked
    a(); // Using '()' to avoid a 'unused-value' warning
}, [&]() {
    // Observable accesses here don't get tracked
    std::cout << "a: " << a << ", b: " << b << std::endl;
});
// Output: "a: 1, b: 1"
obx::runInAction([&]() {
    b = 2;
});
// No output as b is not being observed
obx::runInAction([&]() {
    a = 3;
});
// Output: "a: 3, b: 2"
```

You can disable/enable a *reaction* using its `disable()` and `enable()` methods.
If you want to declare a *reaction* but enable it later on, you can do so: `obx::Reaction r(observerCB, notTrackingCB, false)`.

### Limitations
When calling a non-const method/operator of an *observable* you must use the dereference operator `*` (or the `->` operator) to indicate you want to mutate it:
```c++
obx::Observable str = "Hello"s;
obx::runInAction([&]() {
    str->clear(); // ok
    (*str).clear(); // ok
    str().clear(); // error: ref is const
});
```

When calling a const method/operator you just use the `()` operator:
```c++
const obx::Observable constStr = "Hello"s;
constStr().size(); // ok
constStr->size(); // error, constStr is const

obx::Observable str = "Hello"s;
str().size(); // ok
str->size(); // error, mutating str outside an action
```

