void InspectorNetworkAgent::WillStartFetch(ThreadableLoaderClient* client) {
  DCHECK(!pending_request_);
  pending_request_ = client;
  pending_request_type_ = InspectorPageAgent::kFetchResource;
}
