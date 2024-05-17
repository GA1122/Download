void ContextState::RestoreGlobalState(const ContextState* prev_state) const {
  InitCapabilities(prev_state);
  InitState(prev_state);
}
