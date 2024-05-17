void SetupSimpleShader(const uint8_t* color) {
  static const char* v_shader_str = SHADER(
      attribute vec4 a_Position;
      void main()
      {
         gl_Position = a_Position;
      }
   );

  static const char* f_shader_str = SHADER(
      precision mediump float;
      uniform vec4 u_color;
      void main()
      {
        gl_FragColor = u_color;
      }
  );

  GLuint program = GLTestHelper::LoadProgram(v_shader_str, f_shader_str);
  glUseProgram(program);

  GLuint position_loc = glGetAttribLocation(program, "a_Position");

  GLTestHelper::SetupUnitQuad(position_loc);

  GLuint color_loc = glGetUniformLocation(program, "u_color");
  glUniform4f(
      color_loc,
      color[0] / 255.0f,
      color[1] / 255.0f,
      color[2] / 255.0f,
      color[3] / 255.0f);
}
