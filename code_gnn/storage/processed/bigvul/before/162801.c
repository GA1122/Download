GLuint GetServiceId(const TextureUnit& unit, GLuint target) {
  switch (target) {
    case GL_TEXTURE_2D:
      return Get2dServiceId(unit);
    case GL_TEXTURE_CUBE_MAP:
      return GetCubeServiceId(unit);
    case GL_TEXTURE_RECTANGLE_ARB:
      return GetArbServiceId(unit);
    case GL_TEXTURE_EXTERNAL_OES:
      return GetOesServiceId(unit);
    default:
      NOTREACHED();
      return 0;
  }
}
