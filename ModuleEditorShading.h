#ifndef _MODULE_EDITOR_SHADING_H_
#define _MODULE_EDITOR_SHADING_H_

#include "ModuleEditor.h"

class Viewport;

class ModuleEditorShading : public ModuleEditor
{
public:
    ModuleEditorShading();
    ~ModuleEditorShading();

	bool            Init        ();
	update_status   Update      ();
	bool            CleanUp     ();

public:

    enum Shapes
    {
        SPHERE = 0,
        TORUS,
        CYLINDER,
        SHAPE_COUNT
    };

    Viewport* viewport = nullptr;
    bool auto_rotate   = false;
    unsigned shape     = 0;

private:

    void            LoadShapes (Shapes s);

};

#endif /* _MODULE_EDITOR_SHADING_H_ */
