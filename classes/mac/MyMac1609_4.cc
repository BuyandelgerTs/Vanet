//
// Copyright (C) 2012 David Eckhoff <eckhoff@cs.fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "MyMac1609_4.h"
#include <iterator>

#define DBG_MAC EV
//#define DBG_MAC std::cerr << "[" << simTime().raw() << "] " << myId << " "

Define_Module(MyMac1609_4);

void MyMac1609_4::initialize(int stage) {
	BaseMacLayer::initialize(stage);
	if (stage == 0) {

		phy11p = FindModule<Mac80211pToPhy11pInterface*>::findSubModule(
		             getParentModule());
		assert(phy11p);

		//this is required to circumvent double precision issues with constants from CONST80211p.h
		assert(simTime().getScaleExp() == -12);

		txPower = par("txPower").doubleValue();
		bitrate = par("bitrate");
		checkBitrate(bitrate);

		//mac-adresses
		myMacAddress = intuniform(0,0xFFFFFFFE);
		myId = getParentModule()->getParentModule()->getFullPath();
		//create frequency mappings
		frequency.insert(std::pair<int, double>(Channels::CRIT_SOL, 5.86e9));
		frequency.insert(std::pair<int, double>(Channels::SCH1, 5.87e9));
		frequency.insert(std::pair<int, double>(Channels::SCH2, 5.88e9));
		frequency.insert(std::pair<int, double>(Channels::CCH, 5.89e9));
		frequency.insert(std::pair<int, double>(Channels::SCH3, 5.90e9));
		frequency.insert(std::pair<int, double>(Channels::SCH4, 5.91e9));
		frequency.insert(std::pair<int, double>(Channels::HPPS, 5.92e9));

		//create two edca systems

		myEDCA[type_CCH] = new EDCA(type_CCH,par("queueSize").longValue());
		myEDCA[type_CCH]->myId = myId;
		myEDCA[type_CCH]->myId.append(" CCH");

		myEDCA[type_CCH]->createQueue(2,(((CWMIN_11P+1)/4)-1),(((CWMIN_11P +1)/2)-1),AC_VO);
		myEDCA[type_CCH]->createQueue(3,(((CWMIN_11P+1)/2)-1),CWMIN_11P,AC_VI);
		myEDCA[type_CCH]->createQueue(6,CWMIN_11P,CWMAX_11P,AC_BE);
		myEDCA[type_CCH]->createQueue(9,CWMIN_11P,CWMAX_11P,AC_BK);

		myEDCA[type_SCH] = new EDCA(type_SCH,par("queueSize").longValue());
		myEDCA[type_SCH]->myId = myId;
		myEDCA[type_SCH]->myId.append(" SCH");
		myEDCA[type_SCH]->createQueue(2,(((CWMIN_11P+1)/4)-1),(((CWMIN_11P +1)/2)-1),AC_VO);
		myEDCA[type_SCH]->createQueue(3,(((CWMIN_11P+1)/2)-1),CWMIN_11P,AC_VI);
		myEDCA[type_SCH]->createQueue(6,CWMIN_11P,CWMAX_11P,AC_BE);
		myEDCA[type_SCH]->createQueue(9,CWMIN_11P,CWMAX_11P,AC_BK);

		useSCH = par("useServiceChannel").boolValue();
		if (useSCH) {
			//set the initial service channel
			switch (par("serviceChannel").longValue()) {
				case 1: mySCH = Channels::SCH1; break;
				case 2: mySCH = Channels::SCH2; break;
				case 3: mySCH = Channels::SCH3; break;
				case 4: mySCH = Channels::SCH4; break;
				default: opp_error("Service Channel must be between 1 and 4"); break;
			}
		}

		headerLength = par("headerLength");

		nextMacEvent = new cMessage("next Mac Event");

		if (useSCH) {
            // introduce a little asynchronization between radios, but no more than .3 milliseconds
            uint64_t currenTime = simTime().raw();
            uint64_t switchingTime; // = SWITCHING_INTERVAL_11P.raw();
            define_phase();
            switch (phase) {
                case 1:
                    switchingTime = SWITCHING_INTERVAL_P1.raw();
                    break;
                case 2:
                    switchingTime = SWITCHING_INTERVAL_P2.raw();
                    break;
                case 3:
                    switchingTime = SWITCHING_INTERVAL_P3.raw();
                    break;
                case 4:
                    switchingTime = SWITCHING_INTERVAL_P4.raw();
                    break;
                default:
                    switchingTime = SWITCHING_INTERVAL_11P.raw();
                    break;
            }

            double timeToNextSwitch = (double)(switchingTime - (currenTime % switchingTime)) / simTime().getScale();
            //uint64_t switchingTime = define_phase();
            //double timeToNextSwitch = (double)(switchingTime);
            //timeToNextSwitch = timeToNextSwitch / 1000;

            DBG_MAC << "Phase: " << phase << "; Next switch time after:" << switchingTime << std::endl;
            DBG_MAC << "Time to next channel switch: " << timeToNextSwitch << std::endl;

            if(switchingTime != 0){
                if ((currenTime / switchingTime) % 2 == 0) {
                    setActiveChannel(type_CCH);

                    /* nemelt test */
//                    phy11p->setRadioState(Radio::RX);
                }
                else {
                    setActiveChannel(type_SCH);
                }
            }
            else {
                setActiveChannel(type_CCH);
            }
            // channel switching active
            nextChannelSwitch = new cMessage("Channel Switch");
            simtime_t offset = dblrand() * par("syncOffset").doubleValue();
            scheduleAt(simTime() + offset + timeToNextSwitch, nextChannelSwitch);
        }
        else {
            // no channel switching
            nextChannelSwitch = 0;
            setActiveChannel(type_CCH);
        }

		//stats
		statsReceivedPackets = 0;
		statsReceivedBroadcasts = 0;
		statsSentPackets = 0;
		statsTXRXLostPackets = 0;
		statsSNIRLostPackets = 0;
		statsDroppedPackets = 0;
		statsNumTooLittleTime = 0;
		statsNumInternalContention = 0;
		statsNumBackoff = 0;
		statsSlotsBackoff = 0;
		statsTotalBusyTime = 0;

		idleChannel = true;
		lastBusy = simTime();
		channelIdle(true);

	}
}
void MyMac1609_4::handleSelfMsg(cMessage* msg) {
	if (msg == nextChannelSwitch) {
		ASSERT(useSCH);

		scheduleAt(simTime() + SWITCHING_INTERVAL_11P, nextChannelSwitch);
		switch (activeChannel) {
			case type_CCH:
				DBG_MAC << "CCH --> SCH; Phase: " << phase << "; SCH Number: " << mySCH << std::endl;
				channelBusySelf(false);
				setActiveChannel(type_SCH);
				channelIdle(true);
				phy11p->changeListeningFrequency(frequency[mySCH]);

				/* nemelt test */
//				phy11p->setRadioState(Radio::TX);

				break;
			case type_SCH:
				DBG_MAC << "SCH --> CCH; Phase: " << phase << std::endl;
				channelBusySelf(false);
				setActiveChannel(type_CCH);
				channelIdle(true);
				phy11p->changeListeningFrequency(frequency[Channels::CCH]);

				/* nemelt test */
//				phy11p->setRadioState(Radio::RX);

				break;
		}
		//schedule next channel switch in 50ms

	}
	else if (msg ==  nextMacEvent) {
            //we actually came to the point where we can send a packet
            channelBusySelf(true);
            WaveShortMessage* pktToSend = myEDCA[activeChannel]->initiateTransmit(lastIdle);

            lastAC = mapPriority(pktToSend->getPriority());

            DBG_MAC << "MacEvent received. Trying to send packet with priority" << lastAC << std::endl;

            //send the packet
            Mac80211Pkt* mac = new Mac80211Pkt(pktToSend->getName(), pktToSend->getKind());
            mac->setDestAddr(LAddress::L2BROADCAST);
            mac->setSrcAddr(myMacAddress);
            mac->encapsulate(pktToSend->dup());

            simtime_t sendingDuration = RADIODELAY_11P +  PHY_HDR_PREAMBLE_DURATION +
                                        PHY_HDR_PLCPSIGNAL_DURATION +
                                        ((mac->getBitLength() + PHY_HDR_PSDU_HEADER_LENGTH)/bitrate);
            DBG_MAC << "Sending duration will be" << sendingDuration << std::endl;
            if ((!useSCH) || (timeLeftInSlot() > sendingDuration)) {
                if (useSCH) DBG_MAC << " Time in this slot left: " << timeLeftInSlot() << std::endl;
                // give time for the radio to be in Tx state before transmitting
                phy->setRadioState(Radio::TX); //phy->setRadioState(Radio::TX);

                double freq = (activeChannel == type_CCH) ? frequency[Channels::CCH] : frequency[mySCH];

                attachSignal(mac, simTime()+RADIODELAY_11P, freq);
                MacToPhyControlInfo* phyInfo = dynamic_cast<MacToPhyControlInfo*>(mac->getControlInfo());
                assert(phyInfo);
                DBG_MAC << "Sending a Packet. Frequency " << freq << " Priority" << lastAC << std::endl;

                /* nemelt test */

                sendDelayed(mac, RADIODELAY_11P, lowerLayerOut);
                // nemelt
    //			send(mac, lowerLayerOut);
                statsSentPackets++;
            }
            else {   //not enough time left now
                DBG_MAC << "Too little Time left. This packet cannot be send in this slot." << std::endl;
                statsNumTooLittleTime++;
                //revoke TXOP
                myEDCA[activeChannel]->revokeTxOPs();
                delete mac;
                channelIdle();
                //do nothing. contention will automatically start after channel switch
            }
	}
}

void MyMac1609_4::handleUpperMsg(cMessage* msg) {

	WaveShortMessage* thisMsg;
	if ((thisMsg = dynamic_cast<WaveShortMessage*>(msg)) == NULL) {
		error("WaveMac only accepts WaveShortMessages");
	}

	t_access_category ac = mapPriority(thisMsg->getPriority());

	DBG_MAC << "Received a message from upper layer for channel "
	        << thisMsg->getChannelNumber() << " Access Category (Priority):  "
	        << ac << std::endl;

	t_channel chan;

	//rewrite SCH channel to actual SCH the Mac1609_4 is set to
	if (thisMsg->getChannelNumber() == Channels::SCH1) {
		ASSERT(useSCH);
		thisMsg->setChannelNumber(mySCH);
		chan = type_SCH;
	}

	//put this packet in its queue
	if (thisMsg->getChannelNumber() == Channels::CCH) {
		chan = type_CCH;
	}

	int num = myEDCA[chan]->queuePacket(ac,thisMsg);

	//packet was dropped in Mac
	if (num == -1) {
		statsDroppedPackets++;
		return;
	}

	//if this packet is not at the front of a new queue we dont have to reevaluate times
	DBG_MAC << "sorted packet into queue of EDCA " << chan << " this packet is now at position: " << num << std::endl;

	if (chan == activeChannel) {
		DBG_MAC << "this packet is for the currently active channel" << std::endl;
	}
	else {
		DBG_MAC << "this packet is NOT for the currently active channel" << std::endl;
	}

	/* nemelt test*/
	if(isListenPeriodOver != false && activeChannel != type_CCH) {
        if (num == 1 && idleChannel == true && chan == activeChannel) {

            simtime_t nextEvent = myEDCA[chan]->startContent(lastIdle,guardActive());

            if (nextEvent != -1) {
                if ((!useSCH) || (nextEvent <= nextChannelSwitch->getArrivalTime())) {
                    if (nextMacEvent->isScheduled()) {
                        cancelEvent(nextMacEvent);
                    }
                    scheduleAt(nextEvent,nextMacEvent);
                    DBG_MAC << "Updated nextMacEvent:" << nextMacEvent->getArrivalTime().raw() << std::endl;
                }
                else {
                    DBG_MAC << "Too little time in this interval. Will not schedule nextMacEvent" << std::endl;
                    //it is possible that this queue has an txop. we have to revoke it
                    myEDCA[activeChannel]->revokeTxOPs();
                    statsNumTooLittleTime++;
                }
            }
            else {
                cancelEvent(nextMacEvent);
            }
        }
        if (num == 1 && idleChannel == false && myEDCA[chan]->myQueues[ac].currentBackoff == 0 && chan == activeChannel) {
                myEDCA[chan]->backoff(ac);
        }
	}
}

void MyMac1609_4::handleLowerControl(cMessage* msg) {
	if (msg->getKind() == MacToPhyInterface::TX_OVER) {

		DBG_MAC << "Successfully transmitted a packet on " << lastAC << std::endl;

		phy->setRadioState(Radio::RX); //phy->setRadioState(Radio::RX);

		//message was sent
		//update EDCA queue. go into post-transmit backoff and set cwCur to cwMin
		myEDCA[activeChannel]->postTransmit(lastAC);
		//channel just turned idle.
		//don't set the chan to idle. the PHY layer decides, not us.

		if (guardActive()) {
			opp_error("We shouldnt have sent a packet in guard!");
		}
	}
	else if (msg->getKind() == Mac80211pToPhy11pInterface::CHANNEL_BUSY) {
		channelBusy();
	}
	else if (msg->getKind() == Mac80211pToPhy11pInterface::CHANNEL_IDLE) {
		channelIdle();
	}
	/* NEMELT TEST */
//	else if (msg->getKind() == Mac80211pToPhy11pInterface::SWITCH_RADIO_STATE) {
//	    phy11p->setRadioState(Radio::TX);
//	}
//	else if (msg->getKind() == Mac80211pToPhy11pInterface::LISTEN_PERIOD_OVER) {
//	    phy11p->setRadioState(Radio::TX);
//	    isListenPeriodOver = true;
//	}
	/* END OF NEMELT TEST */
	else if (msg->getKind() == Decider80211p::BITERROR) {
		statsSNIRLostPackets++;
		DBG_MAC << "A packet was not received due to biterrors" << std::endl;
	}
	else if (msg->getKind() == Decider80211p::RECWHILESEND) {
		statsTXRXLostPackets++;
		DBG_MAC << "A packet was not received because we were sending while receiving" << std::endl;
	}
	else if (msg->getKind() == MacToPhyInterface::RADIO_SWITCHING_OVER) {
		DBG_MAC << "Phylayer said radio switching is done" << std::endl;
	}
	else if (msg->getKind() == BaseDecider::PACKET_DROPPED) {
		phy->setRadioState(Radio::RX); //phy->setRadioState(Radio::RX);
		DBG_MAC << "Phylayer said packet was dropped" << std::endl;
	}
	else {
		DBG_MAC << "Invalid control message type (type=NOTHING) : name=" << msg->getName() << " modulesrc=" << msg->getSenderModule()->getFullPath() << "." << std::endl;
		assert(false);
	}
	delete msg;
}

/*
virtual void MyMac1609_4::setActiveChannel(t_channel state) {
	activeChannel = state;
	assert(state == type_CCH || (useSCH && state == type_SCH));
	 //nemelt test
	if (state==type_CCH){
	    phy11p->setRadioState(1);
	    isListenPeriodOver = false;
	}
	if (state==type_SCH){
	    phy11p->setRadioState(0);
	    isListenPeriodOver = true;
	}
	// end of nemelt test
}
*/

/* nemelt test */
void MyMac1609_4::define_phase(){
    try {
        timeval curTime;
        gettimeofday(&curTime,NULL);
        int milli = curTime.tv_usec/1000;
        int rem = milli%100;

        if(rem == 0)
            phase = 4;
        if ((rem > 0) && (rem <= 25))
            phase = 1;
        if((rem > 25) && (rem <= 50))
            phase = 2;
        if((rem > 50) && (rem <= 75))
            phase = 3;
        if((rem > 75) && (rem < 100))
            phase = 4;

    } catch (std::out_of_range e) {
        phase = 2;
    }
}
/* end of nemelt test */
