void GLES2DecoderImpl::RestoreState(const ContextState* prev_state) const {
  TRACE_EVENT1("gpu", "GLES2DecoderImpl::RestoreState",
               "context", logger_.GetLogPrefix());
  RestoreFramebufferBindings();
  state_.RestoreState(prev_state);
}
