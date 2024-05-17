WebGLTexture* WebGL2RenderingContextBase::ValidateTexImageBinding(
    const char* func_name,
    TexImageFunctionID function_id,
    GLenum target) {
  if (function_id == kTexImage3D || function_id == kTexSubImage3D)
    return ValidateTexture3DBinding(func_name, target);
  return ValidateTexture2DBinding(func_name, target);
}
