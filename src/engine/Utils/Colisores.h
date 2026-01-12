#pragma once
#include "raylib.h"
#include "raymath.h"
//void OrientedBoundingBox();




bool RayIntersectsOBB(const Ray& ray, const Vector3& position, const Vector3& rotationDeg, const Vector3& size);

bool RayPlaneIntersection( const Ray& ray, Vector3 planePoint, Vector3 planeNormal, Vector3* outPoint );

Font Carregarfonte();