Response InspectorNetworkAgent::disable() {
  DCHECK(!pending_request_);
  state_->setBoolean(NetworkAgentState::kNetworkAgentEnabled, false);
  state_->setString(NetworkAgentState::kUserAgentOverride, "");
  instrumenting_agents_->removeInspectorNetworkAgent(this);
  resources_data_->Clear();
  known_request_id_map_.clear();
  return Response::OK();
}
