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
package kr.ac.inha.ice.multinet.BuyandelgerTsogoo.classes.nic;

import kr.ac.inha.ice.multinet.BuyandelgerTsogoo.classes.phy.MyPhyLayer80211p;
import kr.ac.inha.ice.multinet.BuyandelgerTsogoo.classes.mac.MyMac1609_4;


//
// This NIC implements an 802.11p network interface card.
//
// @author Buyandelger Tsogoo
//
// @see BaseWaveApplLayer
// @see Mac1609_4
// @see PhyLayer80211p
//
module MyNic80211p
{
    gates:
        input upperLayerIn; // to upper layers
        output upperLayerOut; // from upper layers
        output upperControlOut; // control information 
        input upperControlIn; // control information 
        input radioIn; // radioIn gate for sendDirect

    submodules:
        myphy80211p: MyPhyLayer80211p {
            @display("p=69,218;i=block/process_s");
        }

        mymac1609_4: MyMac1609_4 {
            @display("p=69,82");
        }

    connections:

        radioIn --> myphy80211p.radioIn;

        mymac1609_4.lowerControlOut --> myphy80211p.upperControlIn;
        mymac1609_4.lowerLayerOut --> myphy80211p.upperLayerIn;
        myphy80211p.upperLayerOut --> mymac1609_4.lowerLayerIn;
        myphy80211p.upperControlOut --> mymac1609_4.lowerControlIn;

        mymac1609_4.upperControlIn <-- upperControlIn;
        mymac1609_4.upperLayerIn <-- upperLayerIn;

        mymac1609_4.upperLayerOut --> upperLayerOut;
        mymac1609_4.upperControlOut --> upperControlOut;
}


