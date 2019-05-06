
#ifndef __DECODER_H__
#define __DECODER_H__

#include "./Signals/master.h"
#include "master.h"

class Decoder2SlaveANDMux {
    virtual bool HSELx(int address) = 0;
};

template<typename BUS_size=uint32_t>
class Decoder : public Decoder2SlaveANDMux {
    Master2Decoder * _input;
    bool Decoder2SlaveANDMux::HSELx(int address){

    }
};

#endif
