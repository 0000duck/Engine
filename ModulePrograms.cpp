#include "ModulePrograms.h"

#include "GL/glew.h"

ModulePrograms::ModulePrograms() 
{
    for(unsigned i=0; i< PROGRAM_COUNT; ++i)
    {
        programs[i] = 0;
    }
}

ModulePrograms::~ModulePrograms()
{
}

bool ModulePrograms::Init()
{
    programs[DEFAULT_PROGRAM] = CreateProgram("default.vs", "default.fs");
    programs[GOURAUD_PROGRAM] = CreateProgram("gouraud.vs", "gouraud.fs");

    return programs[DEFAULT_PROGRAM] != 0 && programs[GOURAUD_PROGRAM] != 0;
}

unsigned ModulePrograms::CreateProgram(const char* vshader, const char* fshader)
{
    unsigned program     = 0;
    unsigned vertex_id   = glCreateShader(GL_VERTEX_SHADER);
    unsigned fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

    char* vertex_data   = LoadFile(vshader);
    char* fragment_data = LoadFile(fshader);

	bool ok = Compile(vertex_id, vertex_data);
	ok = ok && Compile(fragment_id, fragment_data);

    free(vertex_data);
    free(fragment_data);

    if(ok)
    {
        program = glCreateProgram();

        glAttachShader(program, vertex_id);
        glAttachShader(program, fragment_id);

        glLinkProgram(program);

        int len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        if(len > 0)
        {
            int written = 0;
            char* info  = (char*)malloc(len);

            glGetProgramInfoLog(program, len, &written, info);

            LOG("Program Log Info: %s", info);

            free(info);
        }

    }

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    return program;
}

bool ModulePrograms::CleanUp()
{
	for(unsigned i=0; i< PROGRAM_COUNT; ++i)
	{
		glDeleteProgram(programs[i]);
	}

	return true;
}

bool ModulePrograms::Compile(unsigned id, char* data)
{
    glShaderSource(id, 1, &data, 0);
    glCompileShader(id);

    int res = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &res);

    if(res == GL_FALSE)
    {
        int len = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

        if(len > 0)
        {
            int written = 0;
            char* info  = (char*)malloc(len);

            glGetShaderInfoLog(id, len, &written, info);

            LOG("Log Info: %s", info);

            free(info);
        }

        return false;
    }

    return true;
}

char* ModulePrograms::LoadFile(const char* file_name)
{
    char* data = nullptr;

	FILE* file = 0;
	fopen_s(&file, file_name, "rb");

	if(file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		data = (char*)malloc(size + 1);

		fread(data, 1, size, file);
		data[size] = 0;

		fclose(file);
	}

    return data;
}
