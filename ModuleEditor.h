#ifndef _MODULE_EDITOR_H_
#define _MODULE_EDITOR_H_

#include "Module.h"

class ModuleEditor : public Module
{
public:
    ModuleEditor();
    virtual ~ModuleEditor();

	bool            Init        ();
	bool            CleanUp     ();

protected:
    void            InitFrame   ();
    void            EndFrame    ();
};

#endif // _MODULE_EDITOR_H_


