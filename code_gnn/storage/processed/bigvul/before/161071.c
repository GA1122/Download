void DevToolsHttpHandler::OnWebSocketMessage(
    int connection_id,
    const std::string& data) {
  ConnectionToClientMap::iterator it =
      connection_to_client_.find(connection_id);
  if (it != connection_to_client_.end())
    it->second->OnMessage(data);
}
