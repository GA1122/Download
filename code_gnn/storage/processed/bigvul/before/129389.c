error::Error GLES2DecoderImpl::HandleAsyncTexImage2DCHROMIUM(
    uint32 immediate_data_size, const cmds::AsyncTexImage2DCHROMIUM& c) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::HandleAsyncTexImage2DCHROMIUM");
  GLenum target = static_cast<GLenum>(c.target);
  GLint level = static_cast<GLint>(c.level);
  GLenum internal_format = static_cast<GLenum>(c.internalformat);
  GLsizei width = static_cast<GLsizei>(c.width);
  GLsizei height = static_cast<GLsizei>(c.height);
  GLint border = static_cast<GLint>(c.border);
  GLenum format = static_cast<GLenum>(c.format);
  GLenum type = static_cast<GLenum>(c.type);
  uint32 pixels_shm_id = static_cast<uint32>(c.pixels_shm_id);
  uint32 pixels_shm_offset = static_cast<uint32>(c.pixels_shm_offset);
  uint32 pixels_size;
  uint32 async_upload_token = static_cast<uint32>(c.async_upload_token);
  uint32 sync_data_shm_id = static_cast<uint32>(c.sync_data_shm_id);
  uint32 sync_data_shm_offset = static_cast<uint32>(c.sync_data_shm_offset);

  base::ScopedClosureRunner scoped_completion_callback;
  if (async_upload_token) {
    base::Closure completion_closure =
        AsyncUploadTokenCompletionClosure(async_upload_token,
                                          sync_data_shm_id,
                                          sync_data_shm_offset);
    if (completion_closure.is_null())
      return error::kInvalidArguments;

    scoped_completion_callback.Reset(completion_closure);
  }

  if (!GLES2Util::ComputeImageDataSizes(
      width, height, format, type, state_.unpack_alignment, &pixels_size, NULL,
      NULL)) {
    return error::kOutOfBounds;
  }
  const void* pixels = NULL;
  if (pixels_shm_id != 0 || pixels_shm_offset != 0) {
    pixels = GetSharedMemoryAs<const void*>(
        pixels_shm_id, pixels_shm_offset, pixels_size);
    if (!pixels) {
      return error::kOutOfBounds;
    }
  }

  TextureManager::DoTextImage2DArguments args = {
    target, level, internal_format, width, height, border, format, type,
    pixels, pixels_size};
  TextureRef* texture_ref;
  if (!texture_manager()->ValidateTexImage2D(
      &state_, "glAsyncTexImage2DCHROMIUM", args, &texture_ref)) {
    return error::kNoError;
  }

  Texture* texture = texture_ref->texture();
  if (!ValidateAsyncTransfer(
      "glAsyncTexImage2DCHROMIUM", texture_ref, target, level, pixels))
    return error::kNoError;

  if (texture->IsDefined()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glAsyncTexImage2DCHROMIUM", "already defined");
    return error::kNoError;
  }

  if (!EnsureGPUMemoryAvailable(pixels_size)) {
    LOCAL_SET_GL_ERROR(
        GL_OUT_OF_MEMORY, "glAsyncTexImage2DCHROMIUM", "out of memory");
    return error::kNoError;
  }

  AsyncTexImage2DParams tex_params = {
      target, level, static_cast<GLenum>(internal_format),
      width, height, border, format, type};
  AsyncMemoryParams mem_params(
      GetSharedMemoryBuffer(c.pixels_shm_id), c.pixels_shm_offset, pixels_size);

  AsyncPixelTransferDelegate* delegate =
      async_pixel_transfer_manager_->CreatePixelTransferDelegate(texture_ref,
                                                                 tex_params);
  texture->SetImmutable(true);

  delegate->AsyncTexImage2D(
      tex_params,
      mem_params,
      base::Bind(&TextureManager::SetLevelInfoFromParams,
                 base::Unretained(texture_manager()),
                 base::Unretained(texture_ref),
                 tex_params));
  return error::kNoError;
}
