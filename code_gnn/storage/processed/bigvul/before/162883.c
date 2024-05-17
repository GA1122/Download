  GLuint SetupColoredVertexProgram() {
    static const char* v_shader_str = SHADER(
        attribute vec4 a_position;
        attribute vec4 a_color;
        varying vec4 v_color;
        void main()
        {
           gl_Position = a_position;
           v_color = a_color;
        }
     );

    static const char* f_shader_str = SHADER(
        precision mediump float;
        varying vec4 v_color;
        void main()
        {
          gl_FragColor = v_color;
        }
    );

    GLuint program = GLTestHelper::LoadProgram(v_shader_str, f_shader_str);
    glUseProgram(program);
    return program;
  }
