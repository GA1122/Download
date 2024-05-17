error::Error GLES2DecoderImpl::HandleReadPixels(uint32_t immediate_data_size,
                                                const volatile void* cmd_data) {
  const char* func_name = "glReadPixels";
  const volatile gles2::cmds::ReadPixels& c =
      *static_cast<const volatile gles2::cmds::ReadPixels*>(cmd_data);
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
  uint32_t pixels_shm_id = c.pixels_shm_id;
  uint32_t pixels_shm_offset = c.pixels_shm_offset;
  uint32_t result_shm_id = c.result_shm_id;
  uint32_t result_shm_offset = c.result_shm_offset;
  GLboolean async = static_cast<GLboolean>(c.async);
  if (width < 0 || height < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "dimensions < 0");
    return error::kNoError;
  }
  typedef cmds::ReadPixels::Result Result;

  PixelStoreParams params;
  if (pixels_shm_id == 0) {
    params = state_.GetPackParams();
  } else {
    params.alignment = state_.pack_alignment;
  }
  uint32_t pixels_size = 0;
  uint32_t unpadded_row_size = 0;
  uint32_t padded_row_size = 0;
  uint32_t skip_size = 0;
  uint32_t padding = 0;
  if (!GLES2Util::ComputeImageDataSizesES3(width, height, 1,
                                           format, type,
                                           params,
                                           &pixels_size,
                                           &unpadded_row_size,
                                           &padded_row_size,
                                           &skip_size,
                                           &padding)) {
    return error::kOutOfBounds;
  }

  uint8_t* pixels = nullptr;
  Buffer* buffer = state_.bound_pixel_pack_buffer.get();
  if (pixels_shm_id == 0) {
    if (!buffer) {
      return error::kInvalidArguments;
    }
    if (!buffer_manager()->RequestBufferAccess(
            error_state_.get(), buffer, func_name, "pixel pack buffer")) {
      return error::kNoError;
    }
    uint32_t size = 0;
    if (!base::CheckAdd(pixels_size + skip_size, pixels_shm_offset)
             .AssignIfValid(&size)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "size + offset overflow");
      return error::kNoError;
    }
    if (static_cast<uint32_t>(buffer->size()) < size) {
      LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glReadPixels",
                         "pixel pack buffer is not large enough");
      return error::kNoError;
    }
    pixels = reinterpret_cast<uint8_t *>(pixels_shm_offset);
    pixels += skip_size;
  } else {
    if (buffer) {
      return error::kInvalidArguments;
    }
    DCHECK_EQ(0u, skip_size);
    pixels = GetSharedMemoryAs<uint8_t*>(
        pixels_shm_id, pixels_shm_offset, pixels_size);
    if (!pixels) {
      return error::kOutOfBounds;
    }
  }

  Result* result = nullptr;
  if (result_shm_id != 0) {
    result = GetSharedMemoryAs<Result*>(
        result_shm_id, result_shm_offset, sizeof(*result));
    if (!result) {
      return error::kOutOfBounds;
    }
    if (result->success != 0) {
      return error::kInvalidArguments;
    }
  }

  if (!validators_->read_pixel_format.IsValid(format)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(func_name, format, "format");
    return error::kNoError;
  }
  if (!validators_->read_pixel_type.IsValid(type)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(func_name, type, "type");
    return error::kNoError;
  }

  if (!CheckBoundReadFramebufferValid(
          func_name, GL_INVALID_FRAMEBUFFER_OPERATION)) {
    return error::kNoError;
  }
  GLenum src_internal_format = GetBoundReadFramebufferInternalFormat();
  if (src_internal_format == 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name, "no valid color image");
    return error::kNoError;
  }
  std::vector<GLenum> accepted_formats;
  std::vector<GLenum> accepted_types;
  switch (src_internal_format) {
    case GL_R8UI:
    case GL_R16UI:
    case GL_R32UI:
    case GL_RG8UI:
    case GL_RG16UI:
    case GL_RG32UI:
    case GL_RGBA8UI:
    case GL_RGB10_A2UI:
    case GL_RGBA16UI:
    case GL_RGBA32UI:
      accepted_formats.push_back(GL_RGBA_INTEGER);
      accepted_types.push_back(GL_UNSIGNED_INT);
      break;
    case GL_R8I:
    case GL_R16I:
    case GL_R32I:
    case GL_RG8I:
    case GL_RG16I:
    case GL_RG32I:
    case GL_RGBA8I:
    case GL_RGBA16I:
    case GL_RGBA32I:
      accepted_formats.push_back(GL_RGBA_INTEGER);
      accepted_types.push_back(GL_INT);
      break;
    case GL_RGB10_A2:
      accepted_formats.push_back(GL_RGBA);
      accepted_types.push_back(GL_UNSIGNED_BYTE);
      accepted_formats.push_back(GL_RGBA);
      accepted_types.push_back(GL_UNSIGNED_INT_2_10_10_10_REV);
      break;
    default:
      accepted_formats.push_back(GL_RGBA);
      {
        GLenum src_type = GetBoundReadFramebufferTextureType();
        switch (src_type) {
          case GL_HALF_FLOAT:
          case GL_HALF_FLOAT_OES:
          case GL_FLOAT:
          case GL_UNSIGNED_INT_10F_11F_11F_REV:
            accepted_types.push_back(GL_FLOAT);
            break;
          default:
            accepted_types.push_back(GL_UNSIGNED_BYTE);
            break;
        }
      }
      break;
  }
  if (!feature_info_->IsWebGLContext()) {
    accepted_formats.push_back(GL_BGRA_EXT);
    accepted_types.push_back(GL_UNSIGNED_BYTE);
  }
  DCHECK_EQ(accepted_formats.size(), accepted_types.size());
  bool format_type_acceptable = false;
  for (size_t ii = 0; ii < accepted_formats.size(); ++ii) {
    if (format == accepted_formats[ii] && type == accepted_types[ii]) {
      format_type_acceptable = true;
      break;
    }
  }
  if (!format_type_acceptable) {
    GLint preferred_format = 0;
    DoGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &preferred_format, 1);
    GLint preferred_type = 0;
    DoGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE, &preferred_type, 1);
    if (format == static_cast<GLenum>(preferred_format) &&
        type == static_cast<GLenum>(preferred_type)) {
      format_type_acceptable = true;
    }
  }
  if (!format_type_acceptable) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
        "format and type incompatible with the current read framebuffer");
    return error::kNoError;
  }
  if (width == 0 || height == 0) {
    return error::kNoError;
  }

  gfx::Size max_size = GetBoundReadFramebufferSize();

  int32_t max_x;
  int32_t max_y;
  if (!base::CheckAdd(x, width).AssignIfValid(&max_x) ||
      !base::CheckAdd(y, height).AssignIfValid(&max_y)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "dimensions out of range");
    return error::kNoError;
  }

  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER(func_name);

  ScopedResolvedFramebufferBinder binder(this, false, true);
  GLenum read_format = GetBoundReadFramebufferInternalFormat();

  gfx::Rect rect(x, y, width, height);   
  gfx::Rect max_rect(max_size);
  if (!max_rect.Contains(rect)) {
    rect.Intersect(max_rect);
    if (!rect.IsEmpty()) {
      std::unique_ptr<ScopedFramebufferCopyBinder> binder;
      if (workarounds()
              .use_copyteximage2d_instead_of_readpixels_on_multisampled_textures &&
          framebuffer_state_.bound_read_framebuffer.get() &&
          framebuffer_state_.bound_read_framebuffer.get()
              ->GetReadBufferIsMultisampledTexture()) {
        binder = std::make_unique<ScopedFramebufferCopyBinder>(this);
      }
      if (y < 0) {
        pixels += static_cast<uint32_t>(-y) * padded_row_size;;
      }
      if (x < 0) {
        uint32_t group_size = GLES2Util::ComputeImageGroupSize(format, type);
        uint32_t leading_bytes = static_cast<uint32_t>(-x) * group_size;
        pixels += leading_bytes;
      }
      for (GLint iy = rect.y(); iy < rect.bottom(); ++iy) {
        bool reset_row_length = false;
        if (iy + 1 == max_y && pixels_shm_id == 0 &&
            workarounds().pack_parameters_workaround_with_pack_buffer &&
            state_.pack_row_length > 0 && state_.pack_row_length < width) {
          api()->glPixelStoreiFn(GL_PACK_ROW_LENGTH, width);
          reset_row_length = true;
        }
        api()->glReadPixelsFn(rect.x(), iy, rect.width(), 1, format, type,
                              pixels);
        if (reset_row_length) {
          api()->glPixelStoreiFn(GL_PACK_ROW_LENGTH, state_.pack_row_length);
        }
        pixels += padded_row_size;
      }
    }
  } else {
    if (async && features().use_async_readpixels &&
        !state_.bound_pixel_pack_buffer.get()) {
      DCHECK(
          !workarounds()
               .use_copyteximage2d_instead_of_readpixels_on_multisampled_textures ||
          !framebuffer_state_.bound_read_framebuffer.get() ||
          !framebuffer_state_.bound_read_framebuffer.get()
               ->GetReadBufferIsMultisampledTexture());
      GLuint buffer = 0;
      api()->glGenBuffersARBFn(1, &buffer);
      api()->glBindBufferFn(GL_PIXEL_PACK_BUFFER_ARB, buffer);
      const GLenum usage_hint =
          gl_version_info().is_angle ? GL_STATIC_DRAW : GL_STREAM_READ;
      api()->glBufferDataFn(GL_PIXEL_PACK_BUFFER_ARB, pixels_size, nullptr,
                            usage_hint);
      GLenum error = api()->glGetErrorFn();
      if (error == GL_NO_ERROR) {
        api()->glReadPixelsFn(x, y, width, height, format, type, 0);
        pending_readpixel_fences_.push(FenceCallback());
        WaitForReadPixels(base::BindOnce(
            &GLES2DecoderImpl::FinishReadPixels, weak_ptr_factory_.GetWeakPtr(),
            width, height, format, type, pixels_shm_id, pixels_shm_offset,
            result_shm_id, result_shm_offset, state_.pack_alignment,
            read_format, buffer));
        api()->glBindBufferFn(GL_PIXEL_PACK_BUFFER_ARB, 0);
        return error::kNoError;
      } else {
        api()->glBindBufferFn(GL_PIXEL_PACK_BUFFER_ARB, 0);
        api()->glDeleteBuffersARBFn(1, &buffer);
      }
    }
    if (pixels_shm_id == 0 &&
        workarounds().pack_parameters_workaround_with_pack_buffer) {
      DCHECK(
          !workarounds()
               .use_copyteximage2d_instead_of_readpixels_on_multisampled_textures);
      if (state_.pack_row_length > 0 && state_.pack_row_length < width) {
        api()->glPixelStoreiFn(GL_PACK_ROW_LENGTH, width);
        for (GLint iy = y; iy < max_y; ++iy) {
          if (iy + 1 == max_y && padding > 0)
            api()->glPixelStoreiFn(GL_PACK_ALIGNMENT, 1);
          api()->glReadPixelsFn(x, iy, width, 1, format, type, pixels);
          if (iy + 1 == max_y && padding > 0)
            api()->glPixelStoreiFn(GL_PACK_ALIGNMENT, state_.pack_alignment);
          pixels += padded_row_size;
        }
        api()->glPixelStoreiFn(GL_PACK_ROW_LENGTH, state_.pack_row_length);
      } else if (padding > 0) {
        if (height > 1)
          api()->glReadPixelsFn(x, y, width, height - 1, format, type, pixels);
        api()->glPixelStoreiFn(GL_PACK_ALIGNMENT, 1);
        pixels += padded_row_size * (height - 1);
        api()->glReadPixelsFn(x, max_y - 1, width, 1, format, type, pixels);
        api()->glPixelStoreiFn(GL_PACK_ALIGNMENT, state_.pack_alignment);
      } else {
        api()->glReadPixelsFn(x, y, width, height, format, type, pixels);
      }
    } else if (
        workarounds()
            .use_copyteximage2d_instead_of_readpixels_on_multisampled_textures &&
        framebuffer_state_.bound_read_framebuffer.get() &&
        framebuffer_state_.bound_read_framebuffer.get()
            ->GetReadBufferIsMultisampledTexture()) {
      ScopedFramebufferCopyBinder binder(this, x, y, width, height);
      api()->glReadPixelsFn(0, 0, width, height, format, type, pixels);
    } else {
      api()->glReadPixelsFn(x, y, width, height, format, type, pixels);
    }
  }
  if (pixels_shm_id != 0) {
    GLenum error = LOCAL_PEEK_GL_ERROR(func_name);
    if (error == GL_NO_ERROR) {
      if (result) {
        result->success = 1;
        result->row_length = static_cast<uint32_t>(rect.width());
        result->num_rows = static_cast<uint32_t>(rect.height());
      }
      FinishReadPixels(width, height, format, type, pixels_shm_id,
                       pixels_shm_offset, result_shm_id, result_shm_offset,
                       state_.pack_alignment, read_format, 0);
    }
  }

  return error::kNoError;
}
