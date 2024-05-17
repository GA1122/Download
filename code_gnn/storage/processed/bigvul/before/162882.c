  void SetUpColoredUnitQuad(const GLfloat* color) {
    GLuint program1 = SetupColoredVertexProgram();
    GLuint position_loc1 = glGetAttribLocation(program1, "a_position");
    GLuint color_loc1 = glGetAttribLocation(program1, "a_color");
    GLTestHelper::SetupUnitQuad(position_loc1);
    GLTestHelper::SetupColorsForUnitQuad(color_loc1, color, GL_STATIC_DRAW);
  }
