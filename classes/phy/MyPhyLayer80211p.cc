//
// Copyright (C) 2011 David Eckhoff <eckhoff@cs.fau.de>
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

/*
 * Based on PhyLayer.cc from Karl Wessel
 * and modifications by Christopher Saloman
 */

#include "MyPhyLayer80211p.h"

#include "Decider80211p.h"
#include "SimplePathlossModel.h"
#include "BreakpointPathlossModel.h"
#include "LogNormalShadowing.h"
#include "JakesFading.h"
#include "PERModel.h"
#include "SimpleObstacleShadowing.h"
#include "TwoRayInterferenceModel.h"
#include "BaseConnectionManager.h"
#include <Consts80211p.h>
#include "AirFrame11p_m.h"
#include "MacToPhyControlInfo.h"

// nemelt test
//#define DBG_MAC EV

Define_Module(MyPhyLayer80211p);

/* nemelt test */
simtime_t MyPhyLayer80211p::setRadioState(int rs){
    simtime_t switchTime = 0;
    Enter_Method_Silent();
    assert(radio);

    if(txOverTimer && txOverTimer->isScheduled()) {
        opp_warning("Switched radio while sending an AirFrame. The effects this would have on the transmission are not simulated by the BasePhyLayer!");
    }

    /* nemelt test */
//    if(rs==1){
//        radio->setSwitchTime(0,1,(simTime().dbl() + 0.025));
////        switchTime = radio->switchTo(rs, simTime());
//        listenPeriodOver = new cMessage("Listen Period is Over", LISTEN_PERIOD_OVER);
//        scheduleAt(simTime().dbl() + 0.025, listenPeriodOver);
////        DBG_MAC << "Simulation time is:" << simTime() << ", Switch time is:0.025, Listen period will be over:" << simTime() + switchTime << std::endl;
//    }
    /* end of nemelt test */
    else{
        if (rs==1) { // TX=0; RX=1(only receive, no transmit)
            radio->setSwitchTime(0,1,(simTime().dbl() + 0.025));
//            switchTime = radio->switchTo(rs, simTime());
//            sendSelfMessage(radioSwitchingOverTimer, simTime() + switchTime);
        }
        else{
            switchTime = radio->switchTo(rs, simTime());

            //invalid switch time, we are probably already switching
            if(switchTime < 0)
                return switchTime;

            // if switching is done in exactly zero-time no extra self-message is scheduled
            if (switchTime == 0.0)
            {
                // TODO: in case of zero-time-switch, send no control-message to mac!
                // maybe call a method finishRadioSwitchingSilent()
                finishRadioSwitching();
            } else
            {
                sendSelfMessage(radioSwitchingOverTimer, simTime() + switchTime);
                /* nemelt test */
                //switchRadioState = new cMessage("Switch Radio State", SWITCH_RADIO_STATE);
                //scheduleAt(simTime() + switchTime, switchRadioState);
                /* end of nemelt test */
            }
        }
    }
    return switchTime;
};
