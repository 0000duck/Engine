#ifndef _MODULE_EDITOR_H_
#define _MODULE_EDITOR_H_

#include "Module.h"

class ModuleEditor : public Module
{
public:
    ModuleEditor();
    virtual ~ModuleEditor();

	bool            Init          () override;
	bool            CleanUp       () override;

    void            DrawDebugData ();

protected:

    void            InitFrame     ();
    void            EndFrame      ();

protected:
    bool   show_axis  = true;
    bool   show_grid  = true;
};

#endif // _MODULE_EDITOR_H_


