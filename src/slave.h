
#ifndef __SLAVE_H__
#define __SLAVE_H__

#include "module.h"
#include "./Signals/master.h"
#include "./Signals/slave_N_mux.h"
#include "decoder.h"

template<typename BUS_size=uint32_t>
class Slave : public Module
{
    typedef SlaveNMuxSignals SlaveSignals;
    typedef Decoder2SlaveANDMux Decoder2Slave;
    
    struct {
        Master2Slave * master;
        Decoder2Slave * HSELx;
        Mux2Slave * HREADYIN;
    } _input;
    SlaveSignals _output[2]; // Old and Current Signals

    const SlaveSignals & const get() {
        return this->_output[Module::current_bus];
    }
};

#endif
