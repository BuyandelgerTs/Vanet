
cplusplus {{
#include "MacPkt_m.h"
}}


class MacPkt;

//
// Defines all fields of an 802.11 MAC frame
//
packet EMSPkt extends MacPkt
{
    int address3;
    int address4;
    int fragmentation; //part of the Frame Control field
    int informationDS; //part of the Frame Control field
    int sequenceControl;
    bool retry;
    simtime_t duration; 	//the expected remaining duration the current transaction
    
    // additional fields
    
}
