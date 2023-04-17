#include <iostream>
#include "NorthWestAngle/NorthWestAngle.h"

int main()
{
    LinearEquations::NorthWestAngle northWestAngle;
    northWestAngle.ReadMatrixFromFile();
    northWestAngle.Process();
}
