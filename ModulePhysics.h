#ifndef _MODULE_PHYSICS_H_
#define _MODULE_PHYSICS_H_

#include "ModuleEditor.h"

#include<vector>

class btCollisionShape;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class Viewport;

class ModulePhysics : public ModuleEditor
{
public:

    ModulePhysics();
    ~ModulePhysics();

	bool            Init();
	update_status   Update();
	bool            CleanUp();

private:
    std::vector<btCollisionShape*> collisionShapes;

	btDefaultCollisionConfiguration*     collisionConfiguration = nullptr;
	btCollisionDispatcher*               dispatcher             = nullptr;
	btBroadphaseInterface*               overlappingPairCache   = nullptr;
	btSequentialImpulseConstraintSolver* solver                 = nullptr;
	btDiscreteDynamicsWorld*             dynamicsWorld          = nullptr;
    Viewport*                            viewport               = nullptr;

};

#endif /* _MODULE_PHYSICS_H_ */
