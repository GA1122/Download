GLES2DecoderPassthroughImpl::GLenumToTextureTarget(GLenum target) {
  switch (target) {
    case GL_TEXTURE_2D:
      return TextureTarget::k2D;
    case GL_TEXTURE_CUBE_MAP:
      return TextureTarget::kCubeMap;
    case GL_TEXTURE_2D_ARRAY:
      return TextureTarget::k2DArray;
    case GL_TEXTURE_3D:
      return TextureTarget::k3D;
    case GL_TEXTURE_2D_MULTISAMPLE:
      return TextureTarget::k2DMultisample;
    case GL_TEXTURE_EXTERNAL_OES:
      return TextureTarget::kExternal;
    case GL_TEXTURE_RECTANGLE_ARB:
      return TextureTarget::kRectangle;
    default:
      return TextureTarget::kUnkown;
  }
}
