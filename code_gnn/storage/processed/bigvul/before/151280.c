Response InspectorPageAgent::setAutoAttachToCreatedPages(bool auto_attach) {
  state_->setBoolean(PageAgentState::kAutoAttachToCreatedPages, auto_attach);
  return Response::OK();
}
