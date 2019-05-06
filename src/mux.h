
#ifndef __MUX_H__
#define __MUX_H__

#include "./Signals/slave_N_mux.h"
#include "slave.h"

template<unsigned nSlaves=3>
class Mux2Master {
    typedef SlaveNMuxSignals MuxSignals;
    virtual MuxSignals & getMuxSignals() = 0;
};

template<typename BUS_size=uint32_t>
class Mux2Slave {
    virtual TransferStatus & getTransferStatus() = 0;
};

template<unsigned nSlaves=3>
class Mux : public Mux2Master, public Mux2Slave
{
    typedef SlaveNMuxSignals MuxSignals;
    typedef Decoder2SlaveANDMux Decoder2Mux;

    struct {
        Slave * _slave[nSlaves];
        Decoder2Mux * HSELx;
    } _input;
    MuxSignals _output[2]; // Old and Current Signals

    MuxSignals & getMuxSignals() {
        return this->_output[this->_input.HSELx->get()][Module::current_bus];
    }
    TransferStatus & getTransferStatus() {
        return ((Mux2Master*)this)->getMuxSignals().
    }
};

#endif
