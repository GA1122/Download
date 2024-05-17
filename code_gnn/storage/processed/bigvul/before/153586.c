GLenum GLES2Implementation::ClientWaitSync(GLsync sync,
                                           GLbitfield flags,
                                           GLuint64 timeout) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glClientWaitSync(" << sync << ", "
                     << flags << ", " << timeout << ")");
  typedef cmds::ClientWaitSync::Result Result;
  auto result = GetResultAs<Result>();
  if (!result) {
    SetGLError(GL_OUT_OF_MEMORY, "ClientWaitSync", "");
    return GL_WAIT_FAILED;
  }
  *result = GL_WAIT_FAILED;
  helper_->ClientWaitSync(ToGLuint(sync), flags, timeout, GetResultShmId(),
                          result.offset());
  WaitForCmd();
  GPU_CLIENT_LOG("returned " << *result);
  CheckGLError();
  return *result;
}
