
#ifndef __MASTER_H__
#define __MASTER_H__

template<typename BUS_size>
class Master;
#include <stdint.h>
#include "module.h"
#include "Signals/master.h"
#include "Signals/slave_N_mux.h"

template<typename BUS_size=uint32_t>
class Master
:   public Module,
    public Master2Slave<BUS_size>,
    public Master2Decoder<BUS_size>
{
    using _Mux2Master = Mux2Master<BUS_size>;
    using _MasterSignals = MasterSignals<BUS_size>;
    using _MasterHADDR = MasterHADDR<BUS_size>;

private:
    _Mux2Master * _input;
    _MasterSignals _output[2]; // Old and Current Signals
    _MasterSignals & output() { return this->_output[!Module::current_bus]; }
    void output(_MasterSignals out)  { this->_output[ Module::current_bus] = out; }
public:
    Master() {}
    ~Master() {}
    virtual void posEdgeClock() override;

    _MasterSignals & getMasterSignals() { return this->output(); }
    _MasterHADDR & getMasterHADDR() { return this->output(); }

    void setMux(_Mux2Master * mux) { this->_input = mux; }
};

template<typename BUS_size>
void Master<BUS_size>::posEdgeClock()  {
    if(this->_input->getMuxSignals().HREADY == PENDING)
        this->output(this->output());
}

#endif
