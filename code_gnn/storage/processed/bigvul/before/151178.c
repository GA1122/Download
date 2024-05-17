void InspectorNetworkAgent::DocumentThreadableLoaderStartedLoadingForClient(
    unsigned long identifier,
    ThreadableLoaderClient* client) {
  if (!client)
    return;
  if (client != pending_request_) {
    DCHECK(!pending_request_);
    return;
  }

  known_request_id_map_.Set(client, identifier);
  String request_id = IdentifiersFactory::RequestId(identifier);
  resources_data_->SetResourceType(request_id, pending_request_type_);
  if (pending_request_type_ == InspectorPageAgent::kXHRResource) {
    resources_data_->SetXHRReplayData(request_id,
                                      pending_xhr_replay_data_.Get());
  }

  ClearPendingRequestData();
}
