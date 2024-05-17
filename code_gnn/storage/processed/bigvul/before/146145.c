void WebGL2RenderingContextBase::texImage3D(GLenum target,
                                            GLint level,
                                            GLint internalformat,
                                            GLsizei width,
                                            GLsizei height,
                                            GLsizei depth,
                                            GLint border,
                                            GLenum format,
                                            GLenum type,
                                            ImageBitmap* bitmap,
                                            ExceptionState& exception_state) {
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "texImage3D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }
  TexImageHelperImageBitmap(kTexImage3D, target, level, internalformat, format,
                            type, 0, 0, 0, bitmap,
                            GetTextureSourceSubRectangle(width, height), depth,
                            unpack_image_height_, exception_state);
}
