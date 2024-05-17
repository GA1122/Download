bool GLES2Implementation::GetBoundPixelTransferBuffer(GLenum target,
                                                      const char* function_name,
                                                      GLuint* buffer_id) {
  *buffer_id = 0;

  switch (target) {
    case GL_PIXEL_PACK_TRANSFER_BUFFER_CHROMIUM:
      *buffer_id = bound_pixel_pack_transfer_buffer_id_;
      break;
    case GL_PIXEL_UNPACK_TRANSFER_BUFFER_CHROMIUM:
      *buffer_id = bound_pixel_unpack_transfer_buffer_id_;
      break;
    default:
      return false;
  }
  if (!*buffer_id) {
    SetGLError(GL_INVALID_OPERATION, function_name, "no buffer bound");
  }
  return true;
}
