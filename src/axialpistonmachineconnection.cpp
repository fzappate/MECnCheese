#include "./axialpistonmachineconnection.h"

ValvePlateConnection::ValvePlateConnection(std::string name,
    AxialPistonMachineControlVolume& pistonControlVolume,
    Chamber& downCh,
    LookUpTable areaTable,
    sunindextype angleIndex,
    sunindextype areaIndex,
    sunindextype hdIndex) : Orifice(name, pistonControlVolume, downCh),
                            areaTable(areaTable),
                            angleIndex(angleIndex),
                            hdIndex(hdIndex)
{

};