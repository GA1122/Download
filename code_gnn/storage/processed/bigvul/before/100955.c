        void createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource, ShaderProgramIndex index)
        {
            GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
            GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            GL_CMD(glShaderSource(vertexShader, 1, &vertexShaderSource, 0))
            GL_CMD(glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0))
            GLuint programID = glCreateProgram();
            GL_CMD(glCompileShader(vertexShader))
            GL_CMD(glCompileShader(fragmentShader))
            GL_CMD(glAttachShader(programID, vertexShader))
            GL_CMD(glAttachShader(programID, fragmentShader))
            GL_CMD(glLinkProgram(programID))
            programs[index].vertexAttrib = glGetAttribLocation(programID, "InVertex");
            programs[index].id = programID;
        }
