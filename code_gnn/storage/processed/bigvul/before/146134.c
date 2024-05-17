void WebGL2RenderingContextBase::texImage2D(GLenum target,
                                            GLint level,
                                            GLint internalformat,
                                            GLenum format,
                                            GLenum type,
                                            ImageData* image_data) {
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "texImage2D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }
  WebGLRenderingContextBase::texImage2D(target, level, internalformat, format,
                                        type, image_data);
}
