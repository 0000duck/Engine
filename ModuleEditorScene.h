#ifndef _MODULE_EDITOR_SCENE_H_
#define _MODULE_EDITOR_SCENE_H_

#include "ModuleEditor.h"

class PanelGOTree;
class PanelProperties;
class Viewport;

class ModuleEditorScene : public ModuleEditor
{
public:
    ModuleEditorScene();
    ~ModuleEditorScene();

	bool            Init        ();
	update_status   Update      ();
	bool            CleanUp     ();

public:

    Viewport*        viewport    = nullptr;
    PanelGOTree*     go_tree     = nullptr;
    PanelProperties* properties  = nullptr;
};

#endif /* _MODULE_EDITOR_SCENE_H_ */
