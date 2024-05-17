void InspectorNetworkAgent::DetachClientRequest(
    ThreadableLoaderClient* client) {
  if (pending_request_ == client) {
    pending_request_ = nullptr;
    if (pending_request_type_ == InspectorPageAgent::kXHRResource) {
      pending_xhr_replay_data_.Clear();
    }
  }
  known_request_id_map_.erase(client);
}
