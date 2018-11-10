#ifndef _MODULE_EDITOR_H_
#define _MODULE_EDITOR_H_

#include "Module.h"

class PanelGOTree;
class PanelProperties;
class Viewport;

class ModuleEditor : public Module
{
public:
    ModuleEditor();
    ~ModuleEditor();

	bool            Init        ();
	update_status   PreUpdate   ();
	update_status   Update      ();
	update_status   PostUpdate  ();
	bool            CleanUp     ();

public:

    Viewport*        viewport    = nullptr;
    PanelGOTree*     go_tree     = nullptr;
    PanelProperties* properties  = nullptr;
};

#endif // _MODULE_EDITOR_H_


