void WebGL2RenderingContextBase::compressedTexImage2D(GLenum target,
                                                      GLint level,
                                                      GLenum internalformat,
                                                      GLsizei width,
                                                      GLsizei height,
                                                      GLint border,
                                                      GLsizei image_size,
                                                      GLintptr offset) {
  if (isContextLost())
    return;
  if (!bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "compressedTexImage2D",
                      "no bound PIXEL_UNPACK_BUFFER");
    return;
  }
  ContextGL()->CompressedTexImage2D(target, level, internalformat, width,
                                    height, border, image_size,
                                    reinterpret_cast<uint8_t*>(offset));
}
