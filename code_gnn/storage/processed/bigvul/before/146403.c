bool WebGLRenderingContextBase::ValidateCompressedTexFormat(
    const char* function_name,
    GLenum format) {
  if (!compressed_texture_formats_.Contains(format)) {
    SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid format");
    return false;
  }
  return true;
}
