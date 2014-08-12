
#ifndef ___MYMAC1609_4_H_
#define ___MYMAC1609_4_H_

#include <simtime.h>
#include <Mac1609_4.h>

class cMessage;

/**
 * @brief
 * Manages timeslots for CCH and SCH listening and sending.
 *
 * @author Buyandelger Tsogoo
 *
 * @ingroup macLayer
 *
 * @see MyWaveAppToMac1609_4Interface
 * @see MyMac1609_4
 * @see MyPhyLayer80211p
 * @see MyDecider80211p
 */
class MyMac1609_4 : public Mac1609_4 {

	public:

        // nemelt
        /** @brief frame kinds */
//        enum Mac80211pMessageKinds {
//          //between MAC layers of two nodes
//          RTS = LAST_BASE_MAC_MESSAGE_KIND, // request to send
//          CTS,                 // clear to send
//          ACK,                 // acknowledgement
//          DATA,
//          BROADCAST,
//          LAST_MAC_80211P_MESSAGE_KIND
//        };

	protected:
//		struct NeighborEntry {
//		        /** @brief The neighbors address.*/
//		        LAddress::L2Type address;
//		        int              fsc;
//		        simtime_t        age;
//		        double           bitrate;
//		        int              phase;
//		        char             direction;
//		        double           velocity;
//		    };
//
//        /** @brief Type for a list of NeighborEntries.*/
//        typedef std::list<NeighborEntry> NeighborList;

        /** @brief Initialization of the module and some variables.*/
        virtual void initialize(int);

        /** @brief Handle messages from upper layer.*/
        virtual void handleUpperMsg(cMessage*);

        /** @brief Handle self messages such as timers.*/
        virtual void handleSelfMsg(cMessage*);

        /** @brief Handle control messages from lower layer.*/
        virtual void handleLowerControl(cMessage* msg);

		/* nemelt test */
		void define_phase();
		/** @brief add a new entry to the neighbor list */
		//void addNeighbor(Mac80211Pkt *af);
		/* end of nemelt test */

		/* nemelt test */
		int phase;
		bool isListenPeriodOver=false;

		const SimTime SWITCHING_INTERVAL_P1 = SimTime().setRaw(25000000000UL);
        const SimTime SWITCHING_INTERVAL_P2 = SimTime().setRaw(50000000000UL);
        const SimTime SWITCHING_INTERVAL_P3 = SimTime().setRaw(75000000000UL);
        const SimTime SWITCHING_INTERVAL_P4 = SimTime().setRaw(100000000000UL);

//        /** @brief A list of this hosts neighbors.*/
//        NeighborList neighbors;
//
//        /** Definition of the states*/
//        enum State {
//          WFDATA = 0, // waiting for data packet
//          QUIET = 1,  // waiting for the communication between two other nodes to end
//          IDLE = 2,   // no packet to send, no packet receiving
//          CONTEND = 3,// contention state (battle for the channel)
//          WFCTS = 4,  // RTS sent, waiting for CTS
//          WFACK = 5,  // DATA packet sent, waiting for ACK
//          BUSY = 6    // during transmission of an ACK or a BROADCAST packet
//        };
        /* end of nemelt test*/
};

#endif /* ___MYMAC1609_4_H_*/
