Response InspectorPageAgent::enable() {
  enabled_ = true;
  state_->setBoolean(PageAgentState::kPageAgentEnabled, true);
  instrumenting_agents_->addInspectorPageAgent(this);
  return Response::OK();
}
