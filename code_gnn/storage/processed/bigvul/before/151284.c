Response InspectorPageAgent::stopScreencast() {
  state_->setBoolean(PageAgentState::kScreencastEnabled, false);
  return Response::OK();
}
