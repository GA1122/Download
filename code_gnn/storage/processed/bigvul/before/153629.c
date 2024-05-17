void GLES2Implementation::Disable(GLenum cap) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glDisable("
                     << GLES2Util::GetStringCapability(cap) << ")");
  bool changed = false;
  if (!state_.SetCapabilityState(cap, false, &changed) || changed) {
    helper_->Disable(cap);
  }
  CheckGLError();
}
