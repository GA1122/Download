error::Error GLES2DecoderImpl::HandlePixelStorei(
    uint32 immediate_data_size, const cmds::PixelStorei& c) {
  GLenum pname = c.pname;
  GLenum param = c.param;
  if (!validators_->pixel_store.IsValid(pname)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM("glPixelStorei", pname, "pname");
    return error::kNoError;
  }
  switch (pname) {
    case GL_PACK_ALIGNMENT:
    case GL_UNPACK_ALIGNMENT:
        if (!validators_->pixel_store_alignment.IsValid(param)) {
            LOCAL_SET_GL_ERROR(
                GL_INVALID_VALUE, "glPixelStorei", "param GL_INVALID_VALUE");
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
        state_.pack_alignment = param;
        break;
    case GL_PACK_REVERSE_ROW_ORDER_ANGLE:
        state_.pack_reverse_row_order = (param != 0);
        break;
    case GL_UNPACK_ALIGNMENT:
        state_.unpack_alignment = param;
        break;
    default:
        NOTREACHED();
        break;
  }
  return error::kNoError;
}