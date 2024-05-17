error::Error GLES2DecoderImpl::HandlePixelStorei(
    uint32 immediate_data_size, const gles2::PixelStorei& c) {
  GLenum pname = c.pname;
  GLenum param = c.param;
  if (!validators_->pixel_store.IsValid(pname)) {
    SetGLErrorInvalidEnum("glPixelStorei", pname, "pname");
    return error::kNoError;
  }
  switch (pname) {
    case GL_PACK_ALIGNMENT:
    case GL_UNPACK_ALIGNMENT:
        if (!validators_->pixel_store_alignment.IsValid(param)) {
            SetGLError(GL_INVALID_VALUE,
                       "glPixelStore", "param GL_INVALID_VALUE");
            return error::kNoError;
        }
        break;
    case GL_UNPACK_FLIP_Y_CHROMIUM:
        unpack_flip_y_ = (param != 0);
        return error::kNoError;
    case GL_UNPACK_PREMULTIPLY_ALPHA_CHROMIUM:
        unpack_premultiply_alpha_ = (param != 0);
        return error::kNoError;
    case GL_UNPACK_UNPREMULTIPLY_ALPHA_CHROMIUM:
        unpack_unpremultiply_alpha_ = (param != 0);
        return error::kNoError;
    default:
        break;
  }
  glPixelStorei(pname, param);
  switch (pname) {
    case GL_PACK_ALIGNMENT:
        pack_alignment_ = param;
        break;
    case GL_PACK_REVERSE_ROW_ORDER_ANGLE:
        break;
    case GL_UNPACK_ALIGNMENT:
        unpack_alignment_ = param;
        break;
    default:
        NOTREACHED();
        break;
  }
  return error::kNoError;
}
