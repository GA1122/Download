size_t GLES2Util::CalcClearBufferfvDataCount(int buffer) {
  switch (buffer) {
    case GL_COLOR:
      return 4;
    case GL_DEPTH:
      return 1;
    default:
      return 0;
   }
 }
