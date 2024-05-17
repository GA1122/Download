error::Error GLES2DecoderImpl::HandleWaitSync(uint32_t immediate_data_size,
                                              const volatile void* cmd_data) {
  const char* function_name = "glWaitSync";
  if (!feature_info_->IsWebGL2OrES3Context())
    return error::kUnknownCommand;
  const volatile gles2::cmds::WaitSync& c =
      *static_cast<const volatile gles2::cmds::WaitSync*>(cmd_data);
  const GLuint sync = static_cast<GLuint>(c.sync);
  const GLbitfield flags = static_cast<GLbitfield>(c.flags);
  const GLuint64 timeout = c.timeout();
  GLsync service_sync = 0;
  if (!group_->GetSyncServiceId(sync, &service_sync)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "invalid sync");
    return error::kNoError;
  }
  if (flags != 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "invalid flags");
    return error::kNoError;
  }
  if (timeout != GL_TIMEOUT_IGNORED) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "invalid timeout");
    return error::kNoError;
  }
  api()->glWaitSyncFn(service_sync, flags, timeout);
  return error::kNoError;
}
