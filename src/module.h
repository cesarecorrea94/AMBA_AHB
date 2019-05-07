
#ifndef __MODULE_H__
#define __MODULE_H__

#include <stdint.h>
#include <list>
#include <cstdio>

class Module {
    static std::list<Module*> modulos;
protected:
    static bool stopSimulation;
    Module() {
        modulos.push_back(this);
    }
public:
    virtual void posEdgeClock() = 0;
    static void simulate() {
        while(!stopSimulation) {
            printf("\t*Clock*\n");
            for (std::list<Module*>::iterator it = modulos.begin();
                it != modulos.end(); ++it) {
                (*it)->posEdgeClock();
            }
        }
    }
};

std::list<Module*> Module::modulos;
bool Module::stopSimulation = false;

//Global Signals
//bool HResetn;
//bool HCLK;

#endif
