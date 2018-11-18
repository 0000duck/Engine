#ifndef _MODULE_EDITOR_SHADING_H_
#define _MODULE_EDITOR_SHADING_H_

#include "ModuleEditor.h"
#include "ModuleModelLoader.h"

class Viewport;

class ModuleEditorShading : public ModuleEditor
{
public:
    ModuleEditorShading();
    ~ModuleEditorShading();

	bool            Init        () override;
	update_status   Update      () override;
	bool            CleanUp     () override;

public:

    enum Shapes
    {
        SPHERE = 0,
        TORUS,
        CUBE,
        CYLINDER,
        SHAPE_COUNT
    };

    Viewport* viewport = nullptr;
    bool auto_rotate   = false;
    unsigned shape     = 0;

private:

    void   LoadShapes (Shapes s, const ModuleModelLoader::Material& material);

private:

};

#endif /* _MODULE_EDITOR_SHADING_H_ */
