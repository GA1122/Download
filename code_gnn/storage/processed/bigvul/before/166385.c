size_t GLES2Util::GetComponentCountForGLTransformType(uint32_t type) {
  switch (type) {
    case GL_TRANSLATE_X_CHROMIUM:
    case GL_TRANSLATE_Y_CHROMIUM:
      return 1;
    case GL_TRANSLATE_2D_CHROMIUM:
      return 2;
    case GL_TRANSLATE_3D_CHROMIUM:
      return 3;
    case GL_AFFINE_2D_CHROMIUM:
    case GL_TRANSPOSE_AFFINE_2D_CHROMIUM:
      return 6;
    case GL_AFFINE_3D_CHROMIUM:
    case GL_TRANSPOSE_AFFINE_3D_CHROMIUM:
      return 12;
    default:
      return 0;
  }
}
