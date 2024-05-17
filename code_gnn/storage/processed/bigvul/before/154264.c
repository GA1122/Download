error::Error GLES2DecoderImpl::HandlePixelStorei(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::PixelStorei& c =
      *static_cast<const volatile gles2::cmds::PixelStorei*>(cmd_data);
  GLenum pname = c.pname;
  GLint param = c.param;
  if (!validators_->pixel_store.IsValid(pname)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM("glPixelStorei", pname, "pname");
    return error::kNoError;
  }
  switch (pname) {
    case GL_PACK_ALIGNMENT:
    case GL_UNPACK_ALIGNMENT:
      if (!validators_->pixel_store_alignment.IsValid(param)) {
        LOCAL_SET_GL_ERROR(
            GL_INVALID_VALUE, "glPixelStorei", "invalid param");
        return error::kNoError;
      }
      break;
    case GL_PACK_ROW_LENGTH:
    case GL_UNPACK_ROW_LENGTH:
    case GL_UNPACK_IMAGE_HEIGHT:
      if (param < 0) {
        LOCAL_SET_GL_ERROR(
            GL_INVALID_VALUE, "glPixelStorei", "invalid param");
        return error::kNoError;
      }
      break;
    case GL_PACK_SKIP_PIXELS:
    case GL_PACK_SKIP_ROWS:
    case GL_UNPACK_SKIP_PIXELS:
    case GL_UNPACK_SKIP_ROWS:
    case GL_UNPACK_SKIP_IMAGES:
      return error::kInvalidArguments;
    default:
      break;
  }
  switch (pname) {
    case GL_PACK_ROW_LENGTH:
      if (state_.bound_pixel_pack_buffer.get())
        api()->glPixelStoreiFn(pname, param);
      break;
    case GL_UNPACK_ROW_LENGTH:
    case GL_UNPACK_IMAGE_HEIGHT:
      if (state_.bound_pixel_unpack_buffer.get())
        api()->glPixelStoreiFn(pname, param);
      break;
    default:
      api()->glPixelStoreiFn(pname, param);
      break;
  }
  switch (pname) {
    case GL_PACK_ALIGNMENT:
      state_.pack_alignment = param;
      break;
    case GL_PACK_ROW_LENGTH:
      state_.pack_row_length = param;
      break;
    case GL_UNPACK_ALIGNMENT:
      state_.unpack_alignment = param;
      break;
    case GL_UNPACK_ROW_LENGTH:
      state_.unpack_row_length = param;
      break;
    case GL_UNPACK_IMAGE_HEIGHT:
      state_.unpack_image_height = param;
      break;
    default:
      NOTREACHED();
      break;
  }
  return error::kNoError;
}
