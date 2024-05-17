        GLint getUniformLocation(ShaderProgramIndex prog, ShaderVariableIndex var, const char* name)
        {
            return programs[prog].vars[var] = glGetUniformLocation(programs[prog].id, name);
        }
