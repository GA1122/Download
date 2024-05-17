void InspectorNetworkAgent::DidFinishFetch(ExecutionContext* context,
                                           ThreadableLoaderClient* client,
                                           const AtomicString& method,
                                           const String& url) {
  ThreadableLoaderClientRequestIdMap::iterator it =
      known_request_id_map_.find(client);
  if (it == known_request_id_map_.end())
    return;
  known_request_id_map_.erase(client);
}
