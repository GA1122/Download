void WebGLRenderingContextBase::texSubImage2D(GLenum target,
                                              GLint level,
                                              GLint xoffset,
                                              GLint yoffset,
                                              GLenum format,
                                              GLenum type,
                                              ImageBitmap* bitmap,
                                              ExceptionState& exception_state) {
  TexImageHelperImageBitmap(
      kTexSubImage2D, target, level, 0, format, type, xoffset, yoffset, 0,
      bitmap, GetTextureSourceSize(bitmap), 1, 0, exception_state);
}
