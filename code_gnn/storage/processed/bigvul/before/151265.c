void InspectorPageAgent::WindowCreated(LocalFrame* created) {
  if (enabled_ &&
      state_->booleanProperty(PageAgentState::kAutoAttachToCreatedPages, false))
    client_->WaitForCreateWindow(created);
}
