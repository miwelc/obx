#pragma once

#include <functional>

namespace obx {

void runInAction(const std::function<void(void)>& f);

}
