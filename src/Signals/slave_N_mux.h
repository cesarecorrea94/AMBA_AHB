
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
    TransferResponse HRESP;
    TransferStatus HREADY;
};

#endif
