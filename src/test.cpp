#include "obx/obx.h"

#include <cstdint>
#include <string>

using namespace std::string_literals;

void actionF(obx::Observable<std::string>& str) {
	obx::Action _;

	str = "bueeenas";
}

int main(int argc, char* argv[]) {
	obx::Observable n = 3;
	obx::Observable str = "maaaalas"s;

	obx::Autorun a([&]() {
		std::cout << "Nuevo valor de n: " << n << std::endl;
	});

	obx::Reaction r([&]() { str(); }, [&]() {
		std::cout << "solo trackeamos str (" << str() << ") pero no n (" << n << ")" << std::endl;
	});

	obx::Computed<uint32_t> computedN([&]() {
		return n + 1;
	});

	std::cout << "computedN: " << computedN << std::endl;

	obx::Computed<std::string> c([&]() {
		return std::to_string(computedN) + " cosasss " + str();
	});

	std::cout << n << std::endl;

	std::cout << "n == 2: " << (n == 2 ? "true" : "false") << std::endl;
	obx::runInAction([&]() {
		n = 2;
	});
	std::cout << "n == 2: " << (n == 2 ? "true" : "false") << std::endl;

	// error: outside action
	// n = 1;

	std::cout << "str size: " << str().size() << std::endl;

	// error: outside an action (-> is a write only operator)
	// std::cout << "str size: " << str->size() << std::endl;

	std::cout << c() << std::endl;

	actionF(str);

	std::cout << c() << " yeaahhaha" << std::endl;

	return 0;
}
