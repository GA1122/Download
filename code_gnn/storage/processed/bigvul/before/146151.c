void WebGL2RenderingContextBase::texSubImage2D(GLenum target,
                                               GLint level,
                                               GLint xoffset,
                                               GLint yoffset,
                                               GLsizei width,
                                               GLsizei height,
                                               GLenum format,
                                               GLenum type,
                                               ImageData* pixels) {
  DCHECK(pixels);
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "texSubImage2D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }
  TexImageHelperImageData(kTexSubImage2D, target, level, 0, 0, format, type, 1,
                          xoffset, yoffset, 0, pixels,
                          GetTextureSourceSubRectangle(width, height), 0);
}
