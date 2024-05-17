void InspectorNetworkAgent::WillDispatchEventSourceEvent(
    ThreadableLoaderClient* event_source,
    const AtomicString& event_name,
    const AtomicString& event_id,
    const String& data) {
  ThreadableLoaderClientRequestIdMap::iterator it =
      known_request_id_map_.find(event_source);
  if (it == known_request_id_map_.end())
    return;
  GetFrontend()->eventSourceMessageReceived(
      IdentifiersFactory::RequestId(it->value), MonotonicallyIncreasingTime(),
      event_name.GetString(), event_id.GetString(), data);
}
