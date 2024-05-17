error::Error GLES2DecoderImpl::HandleAsyncTexSubImage2DCHROMIUM(
    uint32 immediate_data_size, const cmds::AsyncTexSubImage2DCHROMIUM& c) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::HandleAsyncTexSubImage2DCHROMIUM");
  GLenum target = static_cast<GLenum>(c.target);
  GLint level = static_cast<GLint>(c.level);
  GLint xoffset = static_cast<GLint>(c.xoffset);
  GLint yoffset = static_cast<GLint>(c.yoffset);
  GLsizei width = static_cast<GLsizei>(c.width);
  GLsizei height = static_cast<GLsizei>(c.height);
  GLenum format = static_cast<GLenum>(c.format);
  GLenum type = static_cast<GLenum>(c.type);
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

  uint32 data_size;
  if (!GLES2Util::ComputeImageDataSizes(
      width, height, format, type, state_.unpack_alignment, &data_size,
      NULL, NULL)) {
    return error::kOutOfBounds;
  }
  const void* pixels = GetSharedMemoryAs<const void*>(
      c.data_shm_id, c.data_shm_offset, data_size);

  error::Error error = error::kNoError;
  if (!ValidateTexSubImage2D(&error, "glAsyncTexSubImage2DCHROMIUM",
      target, level, xoffset, yoffset, width, height, format, type, pixels)) {
    return error;
  }

  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  Texture* texture = texture_ref->texture();
  if (!ValidateAsyncTransfer(
         "glAsyncTexSubImage2DCHROMIUM", texture_ref, target, level, pixels))
    return error::kNoError;

  if (!texture->SafeToRenderFrom()) {
    if (!texture_manager()->ClearTextureLevel(this, texture_ref,
                                              target, level)) {
      LOCAL_SET_GL_ERROR(
          GL_OUT_OF_MEMORY,
          "glAsyncTexSubImage2DCHROMIUM", "dimensions too big");
      return error::kNoError;
    }
  }

  AsyncTexSubImage2DParams tex_params = {target, level, xoffset, yoffset,
                                              width, height, format, type};
  AsyncMemoryParams mem_params(
      GetSharedMemoryBuffer(c.data_shm_id), c.data_shm_offset, data_size);
  AsyncPixelTransferDelegate* delegate =
      async_pixel_transfer_manager_->GetPixelTransferDelegate(texture_ref);
  if (!delegate) {
    AsyncTexImage2DParams define_params = {target, level,
                                                0, 0, 0, 0, 0, 0};
    texture->GetLevelSize(target, level, &define_params.width,
                                         &define_params.height);
    texture->GetLevelType(target, level, &define_params.type,
                                         &define_params.internal_format);
    delegate = async_pixel_transfer_manager_->CreatePixelTransferDelegate(
        texture_ref, define_params);
    texture->SetImmutable(true);
  }

  delegate->AsyncTexSubImage2D(tex_params, mem_params);
  return error::kNoError;
}
