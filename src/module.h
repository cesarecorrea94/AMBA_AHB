
#ifndef __MODULE_H__
#define __MODULE_H__

#include <stdint.h>
#include <list>
#include <cstdio>

class Module {
    static std::list<Module*> modulos;
protected:
    static bool current_bus;
    Module() {
        modulos.push_back(this);
    }
public:
    virtual void posEdgeClock() = 0;
    static void simulate() {
        for(unsigned nClocks = 0; nClocks < 10; ++nClocks) {
            current_bus = !current_bus;
            printf("\t*Clock*\n");
            for (std::list<Module*>::iterator it = modulos.begin();
                it != modulos.end(); ++it) {
                (*it)->posEdgeClock();
            }
        }
    }
};

//Global Signals
//bool HResetn;
//bool HCLK;

#endif
