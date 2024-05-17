void InspectorNetworkAgent::DidFinishEventSourceRequest(
    ThreadableLoaderClient* event_source) {
  known_request_id_map_.erase(event_source);
  ClearPendingRequestData();
}
