
#ifndef __MODULE_H__
#define __MODULE_H__

#include <stdint.h>

class Module {
protected:
    static bool current_bus;
public:
    virtual void posEdgeClock() = 0;
};

//Global Signals
//bool HResetn;
//bool HCLK;

#endif
