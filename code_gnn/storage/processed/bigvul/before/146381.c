void WebGLRenderingContextBase::TexImage2DBase(GLenum target,
                                               GLint level,
                                               GLint internalformat,
                                               GLsizei width,
                                               GLsizei height,
                                               GLint border,
                                               GLenum format,
                                               GLenum type,
                                               const void* pixels) {
  ContextGL()->TexImage2D(target, level,
                          ConvertTexInternalFormat(internalformat, type), width,
                          height, border, format, type, pixels);
}
