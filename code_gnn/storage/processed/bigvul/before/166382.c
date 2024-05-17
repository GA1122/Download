size_t GLES2Util::GetCoefficientCountForGLPathFragmentInputGenMode(
    uint32_t gen_mode) {
  switch (gen_mode) {
    case GL_EYE_LINEAR_CHROMIUM:
      return 4;
    case GL_OBJECT_LINEAR_CHROMIUM:
      return 3;
    case GL_CONSTANT_CHROMIUM:
      return 1;
    case GL_NONE:
    default:
      return 0;
  }
}
