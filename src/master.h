
#ifndef __MASTER_H__
#define __MASTER_H__

#include "module.h"
#include "./Signals/master.h"
#include "./Signals/slave.h"

template<typename BUS_size=uint32_t>
class Master2Slave {
    virtual const MasterSignals & const get() = 0;//operator()
};

template<typename BUS_size=uint32_t>
class Master2Decoder {
    virtual const MasterHADDR & const get() = 0;
};

template<typename BUS_size=uint32_t>
class Master : public Module, public Master2Slave, public Master2Decoder
{
    Mux2Master * _input;
    MasterSignals _output[2]; // Old and Current Signals

    MasterSignals & Master2Slave::get() { return this->_output[Module::current_bus]; }
    MasterHADDR & Master2Decoder::get() { return this->_output[Module::current_bus]; }
};

#endif
