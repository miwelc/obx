#ifndef __UTILS_H__
#define __UTILS_H__

#include <functional>

namespace obx {

void runInAction(const std::function<void(void)>& f);

}

#endif
