error::Error GLES2DecoderImpl::HandleUnmapBuffer(
    uint32_t immediate_data_size, const volatile void* cmd_data) {
  if (!feature_info_->IsWebGL2OrES3Context()) {
    return error::kUnknownCommand;
  }
  const char* func_name = "glUnmapBuffer";

  const volatile gles2::cmds::UnmapBuffer& c =
      *static_cast<const volatile gles2::cmds::UnmapBuffer*>(cmd_data);
  GLenum target = static_cast<GLenum>(c.target);

  if (!validators_->buffer_target.IsValid(target)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(func_name, target, "target");
    return error::kNoError;
  }

  Buffer* buffer = buffer_manager()->GetBufferInfoForTarget(&state_, target);
  if (!buffer) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name, "no buffer bound");
    return error::kNoError;
  }
  const Buffer::MappedRange* mapped_range = buffer->GetMappedRange();
  if (!mapped_range) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name, "buffer is unmapped");
    return error::kNoError;
  }
  if (!UnmapBufferHelper(buffer, target))
    return error::kLostContext;
  return error::kNoError;
}
