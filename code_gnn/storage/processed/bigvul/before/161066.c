void DevToolsHttpHandler::OnClose(int connection_id) {
  connection_to_client_.erase(connection_id);
}
