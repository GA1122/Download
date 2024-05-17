uint32_t GLES2Util::GLFaceTargetToTextureTarget(uint32_t target) {
  switch (target) {
    case GL_TEXTURE_2D:
    case GL_TEXTURE_EXTERNAL_OES:
    case GL_TEXTURE_RECTANGLE_ARB:
    case GL_TEXTURE_3D:
    case GL_TEXTURE_2D_ARRAY:
      return target;
    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
      return GL_TEXTURE_CUBE_MAP;
    default:
      NOTREACHED();
      return 0;
  }
}
