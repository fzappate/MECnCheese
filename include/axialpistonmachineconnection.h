#include "./orifice.h"
#include "./axialpistonmachinecv.h"
#include "./lookuptable.h"

class ValvePlateConnection : public Orifice
{
    LookUpTable areaTable;
    sunindextype angleIndex; 
    sunindextype areaIndex;
    sunindextype hdIndex;
public:

    ValvePlateConnection(std::string name,
        AxialPistonMachineControlVolume& pistonControlVolume,
        Chamber& downCh,
        LookUpTable area,
        sunindextype angleIndex,
        sunindextype areaIndex,
        sunindextype hdIndex);
};