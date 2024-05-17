error::Error GLES2DecoderImpl::HandleCreateAndConsumeTextureCHROMIUMImmediate(
    uint32_t immediate_data_size,
    const void* cmd_data) {
  const gles2::cmds::CreateAndConsumeTextureCHROMIUMImmediate& c =
      *static_cast<
          const gles2::cmds::CreateAndConsumeTextureCHROMIUMImmediate*>(
          cmd_data);
  GLenum target = static_cast<GLenum>(c.target);
  uint32_t data_size;
  if (!ComputeDataSize(1, sizeof(GLbyte), 64, &data_size)) {
    return error::kOutOfBounds;
  }
  if (data_size > immediate_data_size) {
    return error::kOutOfBounds;
  }
  const GLbyte* mailbox =
      GetImmediateDataAs<const GLbyte*>(c, data_size, immediate_data_size);
  if (!validators_->texture_bind_target.IsValid(target)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(
        "glCreateAndConsumeTextureCHROMIUM", target, "target");
    return error::kNoError;
  }
  if (mailbox == NULL) {
    return error::kOutOfBounds;
  }
  uint32_t client_id = c.client_id;
  DoCreateAndConsumeTextureCHROMIUM(target, mailbox, client_id);
  return error::kNoError;
}
