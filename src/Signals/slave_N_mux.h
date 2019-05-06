
#ifndef __SIGNALS_SLAVE_N_MUX_H__
#define __SIGNALS_SLAVE_N_MUX_H__

#include <stdint.h>

typedef enum {
    OKAY    = 0b0,
    ERROR   = 0b1
} TransferResponse;

typedef enum {
    DONE    = 0b1,
    PENDING = 0b0
} TransferStatus;

template<typename BUS_size=uint32_t>
struct SlaveNMuxSignals
{
    BUS_size HRDATA;
    TransferResponse HRESP = OKAY;
    TransferStatus HREADY = DONE;
};

template<typename BUS_size=uint32_t>
struct Mux2Master {
    using _MuxSignals = SlaveNMuxSignals<BUS_size>;
    virtual _MuxSignals & getMuxSignals() = 0;
};

struct Mux2Slave {
    virtual TransferStatus & getTransferStatus() = 0;
};

#endif
