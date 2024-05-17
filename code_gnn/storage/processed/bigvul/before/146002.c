GLint WebGL2RenderingContextBase::GetMaxTextureLevelForTarget(GLenum target) {
  switch (target) {
    case GL_TEXTURE_3D:
      return max3d_texture_level_;
    case GL_TEXTURE_2D_ARRAY:
      return max_texture_level_;
  }
  return WebGLRenderingContextBase::GetMaxTextureLevelForTarget(target);
}
