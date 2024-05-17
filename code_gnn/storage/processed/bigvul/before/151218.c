Response InspectorNetworkAgent::setBypassServiceWorker(bool bypass) {
  state_->setBoolean(NetworkAgentState::kBypassServiceWorker, bypass);
  return Response::OK();
}
