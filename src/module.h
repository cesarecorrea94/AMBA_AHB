
#ifndef __MODULE_H__
#define __MODULE_H__

#include <stdint.h>

class Module {
    static bool current_bus;
    virtual void posEdgeClock() = 0;
};

bool Module::current_bus = 0;

    //Global Signals
    //bool HResetn;
    //bool HCLK;

#endif
