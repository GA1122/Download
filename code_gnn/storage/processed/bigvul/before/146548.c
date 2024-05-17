void WebGLRenderingContextBase::texImage2D(GLenum target,
                                           GLint level,
                                           GLint internalformat,
                                           GLenum format,
                                           GLenum type,
                                           ImageBitmap* bitmap,
                                           ExceptionState& exception_state) {
  TexImageHelperImageBitmap(kTexImage2D, target, level, internalformat, format,
                            type, 0, 0, 0, bitmap, GetTextureSourceSize(bitmap),
                            1, 0, exception_state);
}
