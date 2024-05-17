error::Error GLES2DecoderImpl::HandleClientWaitSync(
    uint32_t immediate_data_size, const void* cmd_data) {
  if (!unsafe_es3_apis_enabled())
    return error::kUnknownCommand;
  const gles2::cmds::ClientWaitSync& c =
      *static_cast<const gles2::cmds::ClientWaitSync*>(cmd_data);
  GLuint sync = static_cast<GLuint>(c.sync);
  GLbitfield flags = static_cast<GLbitfield>(c.flags);
  GLuint64 timeout = GLES2Util::MapTwoUint32ToUint64(c.timeout_0, c.timeout_1);
  typedef cmds::ClientWaitSync::Result Result;
  Result* result_dst = GetSharedMemoryAs<Result*>(
      c.result_shm_id, c.result_shm_offset, sizeof(*result_dst));
  if (!result_dst) {
    return error::kOutOfBounds;
  }
  if (*result_dst != GL_WAIT_FAILED) {
    return error::kInvalidArguments;
  }
  GLsync service_sync = 0;
  if (!group_->GetSyncServiceId(sync, &service_sync)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "ClientWaitSync", "invalid sync");
    return error::kNoError;
  }
  *result_dst = glClientWaitSync(service_sync, flags, timeout);
  return error::kNoError;
}
