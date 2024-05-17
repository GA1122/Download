WebGLTexture* WebGLRenderingContextBase::ValidateTexImageBinding(
    const char* func_name,
    TexImageFunctionID function_id,
    GLenum target) {
  return ValidateTexture2DBinding(func_name, target);
}
