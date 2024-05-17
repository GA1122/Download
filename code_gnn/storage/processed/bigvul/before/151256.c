void InspectorPageAgent::Restore() {
  if (state_->booleanProperty(PageAgentState::kPageAgentEnabled, false))
    enable();
}
