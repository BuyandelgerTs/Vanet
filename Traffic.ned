package kr.ac.inha.ice.multinet.BuyandelgerTsogoo;

import org.mixim.base.connectionManager.ConnectionManager;
import org.mixim.base.modules.BaseWorldUtility;


network Traffic
{
    int numVehicles;
    double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
    double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
    double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
    @display("bgb=$playgroundSizeX,$playgroundSizeY");
    
    submodules:
        connectionManager: ConnectionManager {
            parameters:
                @display("p=150,0;i=abstract/multicast");
        }
        world: BaseWorldUtility {
            parameters:
                playgroundSizeX = playgroundSizeX;
                playgroundSizeY = playgroundSizeY;
                playgroundSizeZ = playgroundSizeZ;
                @display("p=30,0;i=misc/globe");
        }
        vehicle[numVehicles]: Vehicle;
        
    connections allowunconnected:
}
