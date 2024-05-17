void InspectorNetworkAgent::WillSendEventSourceRequest(
    ThreadableLoaderClient* event_source) {
  DCHECK(!pending_request_);
  pending_request_ = event_source;
  pending_request_type_ = InspectorPageAgent::kEventSourceResource;
}
