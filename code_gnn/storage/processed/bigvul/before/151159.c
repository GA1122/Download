void InspectorNetworkAgent::DidFailFetch(ThreadableLoaderClient* client) {
  known_request_id_map_.erase(client);
}
