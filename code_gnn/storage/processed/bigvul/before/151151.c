void InspectorNetworkAgent::ClearPendingRequestData() {
  if (pending_request_type_ == InspectorPageAgent::kXHRResource)
    pending_xhr_replay_data_.Clear();
  pending_request_ = nullptr;
}
