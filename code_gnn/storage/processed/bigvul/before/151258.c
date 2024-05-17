bool InspectorPageAgent::ScreencastEnabled() {
  return enabled_ &&
         state_->booleanProperty(PageAgentState::kScreencastEnabled, false);
}
