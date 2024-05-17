void WebGL2RenderingContextBase::texSubImage3D(GLenum target,
                                               GLint level,
                                               GLint xoffset,
                                               GLint yoffset,
                                               GLint zoffset,
                                               GLsizei width,
                                               GLsizei height,
                                               GLsizei depth,
                                               GLenum format,
                                               GLenum type,
                                               ImageData* pixels) {
  DCHECK(pixels);
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "texSubImage3D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }
  TexImageHelperImageData(kTexSubImage3D, target, level, 0, 0, format, type,
                          depth, xoffset, yoffset, zoffset, pixels,
                          GetTextureSourceSubRectangle(width, height),
                          unpack_image_height_);
}
