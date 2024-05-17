void WebGL2RenderingContextBase::compressedTexSubImage2D(GLenum target,
                                                         GLint level,
                                                         GLint xoffset,
                                                         GLint yoffset,
                                                         GLsizei width,
                                                         GLsizei height,
                                                         GLenum format,
                                                         GLsizei image_size,
                                                         GLintptr offset) {
  if (isContextLost())
    return;
  if (!bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "compressedTexSubImage2D",
                      "no bound PIXEL_UNPACK_BUFFER");
    return;
  }
  ContextGL()->CompressedTexSubImage2D(target, level, xoffset, yoffset, width,
                                       height, format, image_size,
                                       reinterpret_cast<uint8_t*>(offset));
}
