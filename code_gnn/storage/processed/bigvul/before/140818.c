error::Error GLES2DecoderImpl::HandleReadPixels(uint32 immediate_data_size,
                                                const void* cmd_data) {
  const gles2::cmds::ReadPixels& c =
      *static_cast<const gles2::cmds::ReadPixels*>(cmd_data);
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::HandleReadPixels");
  error::Error fbo_error = WillAccessBoundFramebufferForRead();
  if (fbo_error != error::kNoError)
    return fbo_error;
  GLint x = c.x;
  GLint y = c.y;
  GLsizei width = c.width;
  GLsizei height = c.height;
  GLenum format = c.format;
  GLenum type = c.type;
  GLboolean async = static_cast<GLboolean>(c.async);
  if (width < 0 || height < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glReadPixels", "dimensions < 0");
    return error::kNoError;
  }
  typedef cmds::ReadPixels::Result Result;
  uint32 pixels_size;
  if (!GLES2Util::ComputeImageDataSizes(
      width, height, 1, format, type, state_.pack_alignment, &pixels_size,
      NULL, NULL)) {
    return error::kOutOfBounds;
  }
  void* pixels = GetSharedMemoryAs<void*>(
      c.pixels_shm_id, c.pixels_shm_offset, pixels_size);
  if (!pixels) {
    return error::kOutOfBounds;
  }
  Result* result = NULL;
  if (c.result_shm_id != 0) {
    result = GetSharedMemoryAs<Result*>(
        c.result_shm_id, c.result_shm_offset, sizeof(*result));
    if (!result) {
      return error::kOutOfBounds;
    }
  }

  if (!validators_->read_pixel_format.IsValid(format)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM("glReadPixels", format, "format");
    return error::kNoError;
  }
  if (!validators_->read_pixel_type.IsValid(type)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM("glReadPixels", type, "type");
    return error::kNoError;
  }
  if ((format != GL_RGBA && format != GL_BGRA_EXT && format != GL_RGB &&
      format != GL_ALPHA) || type != GL_UNSIGNED_BYTE) {
    GLint preferred_format = 0;
    DoGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &preferred_format);
    GLint preferred_type = 0;
    DoGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE, &preferred_type);
    if (format != static_cast<GLenum>(preferred_format) ||
        type != static_cast<GLenum>(preferred_type)) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION, "glReadPixels", "format and type incompatible "
          "with the current read framebuffer");
      return error::kNoError;
    }
  }
  if (width == 0 || height == 0) {
    return error::kNoError;
  }

  gfx::Size max_size = GetBoundReadFrameBufferSize();

  int32 max_x;
  int32 max_y;
  if (!SafeAddInt32(x, width, &max_x) || !SafeAddInt32(y, height, &max_y)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glReadPixels", "dimensions out of range");
    return error::kNoError;
  }

  if (!CheckBoundReadFramebufferColorAttachment("glReadPixels")) {
    return error::kNoError;
  }

  if (!CheckBoundFramebuffersValid("glReadPixels")) {
    return error::kNoError;
  }

  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER("glReadPixels");

  ScopedResolvedFrameBufferBinder binder(this, false, true);

  if (x < 0 || y < 0 || max_x > max_size.width() || max_y > max_size.height()) {
    uint32 temp_size;
    uint32 unpadded_row_size;
    uint32 padded_row_size;
    if (!GLES2Util::ComputeImageDataSizes(
        width, 2, 1, format, type, state_.pack_alignment, &temp_size,
        &unpadded_row_size, &padded_row_size)) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_VALUE, "glReadPixels", "dimensions out of range");
      return error::kNoError;
    }

    GLint dest_x_offset = std::max(-x, 0);
    uint32 dest_row_offset;
    if (!GLES2Util::ComputeImageDataSizes(
        dest_x_offset, 1, 1, format, type, state_.pack_alignment,
        &dest_row_offset, NULL, NULL)) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_VALUE, "glReadPixels", "dimensions out of range");
      return error::kNoError;
    }

    int8* dst = static_cast<int8*>(pixels);
    GLint read_x = std::max(0, x);
    GLint read_end_x = std::max(0, std::min(max_size.width(), max_x));
    GLint read_width = read_end_x - read_x;
    for (GLint yy = 0; yy < height; ++yy) {
      GLint ry = y + yy;

      memset(dst, 0, unpadded_row_size);

      if (ry >= 0 && ry < max_size.height() && read_width > 0) {
        glReadPixels(
            read_x, ry, read_width, 1, format, type, dst + dest_row_offset);
      }
      dst += padded_row_size;
    }
  } else {
    if (async && features().use_async_readpixels) {
      GLuint buffer = 0;
      glGenBuffersARB(1, &buffer);
      glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, buffer);
      const GLenum usage_hint = feature_info_->gl_version_info().is_angle ?
          GL_STATIC_DRAW : GL_STREAM_READ;
      glBufferData(GL_PIXEL_PACK_BUFFER_ARB, pixels_size, NULL, usage_hint);
      GLenum error = glGetError();
      if (error == GL_NO_ERROR) {
        glReadPixels(x, y, width, height, format, type, 0);
        pending_readpixel_fences_.push(linked_ptr<FenceCallback>(
            new FenceCallback()));
        WaitForReadPixels(base::Bind(
            &GLES2DecoderImpl::FinishReadPixels,
            base::internal::SupportsWeakPtrBase::StaticAsWeakPtr
            <GLES2DecoderImpl>(this),
            c, buffer));
        glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);
        return error::kNoError;
      } else {
        glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);
        glDeleteBuffersARB(1, &buffer);
      }
    }
    glReadPixels(x, y, width, height, format, type, pixels);
  }
  GLenum error = LOCAL_PEEK_GL_ERROR("glReadPixels");
  if (error == GL_NO_ERROR) {
    if (result != NULL) {
      *result = true;
    }
    FinishReadPixels(c, 0);
  }

  return error::kNoError;
}
