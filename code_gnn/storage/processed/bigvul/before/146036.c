bool WebGL2RenderingContextBase::ValidateTexFuncLayer(const char* function_name,
                                                      GLenum tex_target,
                                                      GLint layer) {
  if (layer < 0) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "layer out of range");
    return false;
  }
  switch (tex_target) {
    case GL_TEXTURE_3D:
      if (layer > max3d_texture_size_ - 1) {
        SynthesizeGLError(GL_INVALID_VALUE, function_name,
                          "layer out of range");
        return false;
      }
      break;
    case GL_TEXTURE_2D_ARRAY:
      if (layer > max_array_texture_layers_ - 1) {
        SynthesizeGLError(GL_INVALID_VALUE, function_name,
                          "layer out of range");
        return false;
      }
      break;
    default:
      NOTREACHED();
      return false;
  }
  return true;
}
