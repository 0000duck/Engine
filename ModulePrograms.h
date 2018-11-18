#ifndef __ModulePrograms_h__
#define __ModulePrograms_h__

#include "Module.h"

class ModulePrograms : public Module
{
public:
    ModulePrograms();
    ~ModulePrograms();

	bool Init   ();
    bool CleanUp();

private:

    unsigned CreateProgram(const char* vshader, const char* fshader);
    char*    LoadFile(const char* file_name);
    bool     Compile(unsigned id, char* data);

public:
    enum Program
    {
        DEFAULT_PROGRAM = 0,
        FLAT_PROGRAM,
        GOURAUD_PROGRAM,
        PHONG_PROGRAM,
        BLINN_PROGRAM,
        PROGRAM_COUNT
    };

    unsigned programs[PROGRAM_COUNT];
};

#endif /* __ModulePrograms_h__ */
