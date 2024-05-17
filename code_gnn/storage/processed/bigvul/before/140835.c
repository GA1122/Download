error::Error GLES2DecoderImpl::HandleWaitSync(
    uint32_t immediate_data_size, const void* cmd_data) {
  if (!unsafe_es3_apis_enabled())
    return error::kUnknownCommand;
  const gles2::cmds::WaitSync& c =
      *static_cast<const gles2::cmds::WaitSync*>(cmd_data);
  GLuint sync = static_cast<GLuint>(c.sync);
  GLbitfield flags = static_cast<GLbitfield>(c.flags);
  GLuint64 timeout = GLES2Util::MapTwoUint32ToUint64(c.timeout_0, c.timeout_1);
  GLsync service_sync = 0;
  if (!group_->GetSyncServiceId(sync, &service_sync)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "WaitSync", "invalid sync");
    return error::kNoError;
  }
  glWaitSync(service_sync, flags, timeout);
  return error::kNoError;
}
