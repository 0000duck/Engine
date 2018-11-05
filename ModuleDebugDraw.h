#ifndef _MODULE_DEBUGDRAW_H_
#define _MODULE_DEBUGDRAW_H_

#include "Module.h"

class DDRenderInterfaceCoreGL;

class ModuleDebugDraw : public Module
{

public:

    ModuleDebugDraw();
    ~ModuleDebugDraw();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

    static DDRenderInterfaceCoreGL* implementation;
	unsigned total_time;
};

#endif /* _MODULE_DEBUGDRAW_H_ */
