#include "./orifice.h"
#include "./axialpistonmachinecv.h"
#include "./lookuptable.h"

class AxialPistonMachineValvePlateConnection : public Orifice
{
    AxialPistonMachineControlVolume& pistonControlVolume;
    LookUpTable areaTable;

    // Index of the angle column in areaTable
    sunindextype angleIndex; 
    sunindextype areaIndex;
    sunindextype hdIndex;

    sunindextype interpLowerIndex;
    sunindextype interpCoeff;

public:

    AxialPistonMachineValvePlateConnection(std::string name,
        AxialPistonMachineControlVolume& pistonControlVolume,
        Chamber& downCh,
        LookUpTable& areaTable,
        sunindextype angleIndex,
        sunindextype areaIndex,
        sunindextype hdIndex);

    void CalculateRHS() override;

    void CalculateArea();

    void FindInterpolationIndexes();
};