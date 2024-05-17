bool GLES2DecoderImpl::ValidateCompressedTexFuncData(const char* function_name,
                                                     GLsizei width,
                                                     GLsizei height,
                                                     GLsizei depth,
                                                     GLenum format,
                                                     GLsizei size,
                                                     const GLvoid* data) {
  GLsizei bytes_required = 0;
  if (!GetCompressedTexSizeInBytes(function_name, width, height, depth, format,
                                   &bytes_required, state_.GetErrorState())) {
    return false;
  }

  if (size != bytes_required) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, function_name, "size is not correct for dimensions");
    return false;
  }

  Buffer* buffer = state_.bound_pixel_unpack_buffer.get();
  if (buffer &&
      !buffer_manager()->RequestBufferAccess(
          state_.GetErrorState(), buffer, reinterpret_cast<GLintptr>(data),
          static_cast<GLsizeiptr>(bytes_required), function_name,
          "pixel unpack buffer")) {
    return false;
  }

  return true;
}
