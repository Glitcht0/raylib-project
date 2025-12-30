#include "Colisores.h"


bool RayPlaneIntersection( const Ray& ray, Vector3 planePoint, Vector3 planeNormal, Vector3* outPoint ) {
    float denom = Vector3DotProduct(ray.direction, planeNormal);

    if (fabs(denom) < 0.0001f)
        return false;

    float t = Vector3DotProduct(
        Vector3Subtract(planePoint, ray.position),
        planeNormal
    ) / denom;

    if (t < 0)
        return false;

    *outPoint = Vector3Add(
        ray.position,
        Vector3Scale(ray.direction, t)
    );

    return true;
}
