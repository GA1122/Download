void GLES2Implementation::PixelStorei(GLenum pname, GLint param) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glPixelStorei("
                     << GLES2Util::GetStringPixelStore(pname) << ", " << param
                     << ")");
  switch (pname) {
    case GL_PACK_ALIGNMENT:
    case GL_UNPACK_ALIGNMENT:
      if (param != 1 && param != 2 && param != 4 && param != 8) {
        SetGLError(GL_INVALID_VALUE, "glPixelStorei", "invalid param");
        return;
      }
      break;
    case GL_PACK_ROW_LENGTH:
    case GL_PACK_SKIP_PIXELS:
    case GL_PACK_SKIP_ROWS:
    case GL_UNPACK_IMAGE_HEIGHT:
    case GL_UNPACK_SKIP_IMAGES:
      if (capabilities_.major_version < 3) {
        SetGLError(GL_INVALID_ENUM, "glPixelStorei", "invalid pname");
        return;
      }
      if (param < 0) {
        SetGLError(GL_INVALID_VALUE, "glPixelStorei", "invalid param");
        return;
      }
      break;
    case GL_UNPACK_ROW_LENGTH:
    case GL_UNPACK_SKIP_ROWS:
    case GL_UNPACK_SKIP_PIXELS:
      if (param < 0) {
        SetGLError(GL_INVALID_VALUE, "glPixelStorei", "invalid param");
        return;
      }
      break;
    default:
      SetGLError(GL_INVALID_ENUM, "glPixelStorei", "invalid pname");
      return;
  }
  switch (pname) {
    case GL_PACK_ALIGNMENT:
      pack_alignment_ = param;
      break;
    case GL_PACK_ROW_LENGTH:
      pack_row_length_ = param;
      break;
    case GL_PACK_SKIP_PIXELS:
      pack_skip_pixels_ = param;
      return;
    case GL_PACK_SKIP_ROWS:
      pack_skip_rows_ = param;
      return;
    case GL_UNPACK_ALIGNMENT:
      unpack_alignment_ = param;
      break;
    case GL_UNPACK_ROW_LENGTH:
      unpack_row_length_ = param;
      if (capabilities_.major_version < 3) {
        return;
      }
      break;
    case GL_UNPACK_IMAGE_HEIGHT:
      unpack_image_height_ = param;
      break;
    case GL_UNPACK_SKIP_ROWS:
      unpack_skip_rows_ = param;
      return;
    case GL_UNPACK_SKIP_PIXELS:
      unpack_skip_pixels_ = param;
      return;
    case GL_UNPACK_SKIP_IMAGES:
      unpack_skip_images_ = param;
      return;
    default:
      NOTREACHED();
      break;
  }
  helper_->PixelStorei(pname, param);
  CheckGLError();
}
