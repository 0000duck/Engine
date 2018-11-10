#ifndef _MODULE_EDITOR_H_
#define _MODULE_EDITOR_H_

#include "Module.h"

class ArcBall;
class PanelGOTree;

class ModuleEditor : public Module
{
public:
    ModuleEditor();
    ~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
    ArcBall* camera_ctrl;
    PanelGOTree* go_tree;
};

#endif // _MODULE_EDITOR_H_


