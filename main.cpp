#include "subframe/subframe.h"
#include <iostream>
void test() {
    constexpr auto m  { GNSS_data::fGlonassLine1["m"]};
    GNSS_data::GlonassLine1 gs;
    gs["tk"] = 3;


    gs["m"] = 4;
    for (auto i: {"tk", "m"}) {
        std::cout << i << ": " << gs[i] << std::endl;
    }

    for (auto i: gs) {
        std::cout << i.field.fieldName << ": " << i << "; ";
    }
}
int main() {
    test();
}