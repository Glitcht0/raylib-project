#include "Colisores.h"


//Retorn true se o raio intersecta a OBB definida por posição, rotação (em graus) e tamanho
bool RayIntersectsOBB(const Ray& ray, const Vector3& position, const Vector3& rotationDeg, const Vector3& size) {
    Matrix mat =
        MatrixMultiply(
            MatrixRotateXYZ((Vector3){
                rotationDeg.x * DEG2RAD,
                rotationDeg.y * DEG2RAD,
                rotationDeg.z * DEG2RAD
            }),
            MatrixTranslate(position.x, position.y, position.z)
        );

    Matrix inv = MatrixInvert(mat);

    Ray localRay;
    localRay.position = Vector3Transform(ray.position, inv);

    Vector3 d = ray.direction;
    localRay.direction = Vector3Normalize((Vector3){
        d.x * inv.m0 + d.y * inv.m4 + d.z * inv.m8,
        d.x * inv.m1 + d.y * inv.m5 + d.z * inv.m9,
        d.x * inv.m2 + d.y * inv.m6 + d.z * inv.m10
    });

    BoundingBox box = {
        (Vector3){ -size.x/2, -size.y/2, -size.z/2 },
        (Vector3){  size.x/2,  size.y/2,  size.z/2 }
    };

    return GetRayCollisionBox(localRay, box).hit;
}



/*
void OrientedBoundingBox( Camera3D camera, std::vector<GameObject*>& objects) {
    
    Ray ray = GetMouseRay(GetMousePosition(), camera);
    bool clickedSomething = false;

    for (GameObject* obj : objects){
        Cube* cube = dynamic_cast<Cube*>(obj);
        if (!cube) continue;

        Matrix mat = MatrixMultiply(MatrixRotateXYZ((Vector3){
            cube->rotation.x * DEG2RAD,
            cube->rotation.y * DEG2RAD,
            cube->rotation.z * DEG2RAD
        }), MatrixTranslate(cube->position.x, cube->position.y, cube->position.z));

        Matrix inv = MatrixInvert(mat);

        Ray localRay;
        localRay.position  = Vector3Transform(ray.position, inv);
        Vector3 dir = ray.direction;

        localRay.direction = Vector3Normalize((Vector3){
            dir.x * inv.m0 + dir.y * inv.m4 + dir.z * inv.m8,
            dir.x * inv.m1 + dir.y * inv.m5 + dir.z * inv.m9,
            dir.x * inv.m2 + dir.y * inv.m6 + dir.z * inv.m10
        });
        
        
        BoundingBox box;
        box.min = (Vector3){
            -cube->size.x/2,
            -cube->size.y/2,
            -cube->size.z/2
        };
        box.max = (Vector3){
            cube->size.x/2,
            cube->size.y/2,
            cube->size.z/2
        };


        RayCollision hit = GetRayCollisionBox(localRay, box);

        if (hit.hit) {
            cube->is_selected = true;
            clickedSomething = true;
        } else {
            cube->is_selected = false;
        }
        

        // clicou no vazio
        if (!clickedSomething) {
            for (GameObject* obj : objects) {
                obj->is_selected = false;
            }
        }
    }

}
    */