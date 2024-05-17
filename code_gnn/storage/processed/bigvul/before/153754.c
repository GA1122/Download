GLboolean GLES2Implementation::IsEnabled(GLenum cap) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glIsEnabled("
                     << GLES2Util::GetStringCapability(cap) << ")");
  bool state = false;
  if (!state_.GetEnabled(cap, &state)) {
    typedef cmds::IsEnabled::Result Result;
    auto result = GetResultAs<Result>();
    if (!result) {
      return GL_FALSE;
    }
    *result = 0;
    helper_->IsEnabled(cap, GetResultShmId(), result.offset());
    WaitForCmd();
    state = (*result) != 0;
  }

  GPU_CLIENT_LOG("returned " << state);
  CheckGLError();
  return state;
}
