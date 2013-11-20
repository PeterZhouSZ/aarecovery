#ifndef __AA_RECOVERY_H__
#define __AA_RECOVERY_H__

#include "ppmimage.h"
#include "vector2d.h"
#include "vector3d.h"
#include "netpbmloader.h"

#include <vector>
#include <cmath>

const float SIGMA_D = 0.2 * COLOR_LEVELS;
const float SIGMA_E = 0.05;// * COLOR_LEVELS;

class AARecovery {
    public:
        static PPMImage PerformAA(const PPMImage& original, const PPMImage& filtered);
    private:
        static float Sobel(const PPMImage& neighbors);
        static Vector3D MaximumVarianceDirection(PPMImage& neighbors);
        static bool FindExtremeColors(const PPMImage& neighbors, const Vector3D& varDir, Vector2D& maxColorPos, Vector2D& minColorPos);
};

#endif
