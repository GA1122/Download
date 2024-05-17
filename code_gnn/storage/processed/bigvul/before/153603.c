GLuint GLES2Implementation::CreateImageCHROMIUMHelper(ClientBuffer buffer,
                                                      GLsizei width,
                                                      GLsizei height,
                                                      GLenum internalformat) {
  if (width <= 0) {
    SetGLError(GL_INVALID_VALUE, "glCreateImageCHROMIUM", "width <= 0");
    return 0;
  }

  if (height <= 0) {
    SetGLError(GL_INVALID_VALUE, "glCreateImageCHROMIUM", "height <= 0");
    return 0;
  }

  if (!CreateImageValidInternalFormat(internalformat, capabilities_)) {
    SetGLError(GL_INVALID_VALUE, "glCreateImageCHROMIUM", "invalid format");
    return 0;
  }

  FlushHelper();

  int32_t image_id = gpu_control_->CreateImage(buffer, width, height);
  if (image_id < 0) {
    SetGLError(GL_OUT_OF_MEMORY, "glCreateImageCHROMIUM", "image_id < 0");
    return 0;
  }
  return image_id;
}
