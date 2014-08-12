package kr.ac.inha.ice.multinet.BuyandelgerTsogoo;

import kr.ac.inha.ice.multinet.BuyandelgerTsogoo.classes.nic.MyNic80211p;
import org.mixim.base.modules.*;

module Vehicle
{
    parameters:
        string applType;
        //int phase;
        string mobilityType;
        @display("bgb=403,287;i=abstract/person;r=500");
    gates:
        input radioIn;
    submodules:
        arp: BaseArp {
            @display("p=138,77;b=24,24,rect,blue;i=block/process");
        }
        mobility: <mobilityType> like IMobility {
            parameters:
                @display("p=132,154;i=block/cogwheel");
        }
        appl: <applType> like IBaseApplLayer {
            parameters:
                @display("p=60,36;i=app");
        }
        nic: MyNic80211p {
            parameters:
                @display("p=61,185;i=block/ifcard");
        }

    connections:
        nic.upperLayerOut --> appl.lowerLayerIn;
        nic.upperLayerIn <-- appl.lowerLayerOut;
        nic.upperControlOut --> { @display("ls=red;m=m,70,0,70,0"); } --> appl.lowerControlIn;
        nic.upperControlIn <-- { @display("ls=red;m=m,70,0,70,0"); } <-- appl.lowerControlOut;

        radioIn --> nic.radioIn;

}
