bool WebGLRenderingContextBase::CanUseTexImageByGPU(GLenum format,
                                                    GLenum type) {
#if defined(OS_MACOSX)
  if (type == GL_UNSIGNED_SHORT_5_5_5_1)
    return false;
#endif
  if (format == GL_RED || format == GL_RED_INTEGER)
    return false;

#if defined(OS_ANDROID)
  if (type == GL_FLOAT)
    return false;
#endif

  if (type == GL_HALF_FLOAT_OES)
    return false;

  return true;
}
