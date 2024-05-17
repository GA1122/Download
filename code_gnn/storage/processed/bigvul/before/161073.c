void DevToolsHttpHandler::OnWebSocketRequest(
    int connection_id,
    const net::HttpServerRequestInfo& request) {
  if (!thread_)
    return;

  if (base::StartsWith(request.path, browser_guid_,
                       base::CompareCase::SENSITIVE)) {
    scoped_refptr<DevToolsAgentHost> browser_agent =
        DevToolsAgentHost::CreateForBrowser(
            thread_->task_runner(),
            base::Bind(&DevToolsSocketFactory::CreateForTethering,
                       base::Unretained(socket_factory_.get())));
    connection_to_client_[connection_id].reset(new DevToolsAgentHostClientImpl(
        thread_->message_loop(), server_wrapper_.get(), connection_id,
        browser_agent));
    AcceptWebSocket(connection_id, request);
    return;
  }

  if (!base::StartsWith(request.path, kPageUrlPrefix,
                        base::CompareCase::SENSITIVE)) {
    Send404(connection_id);
    return;
  }

  std::string target_id = request.path.substr(strlen(kPageUrlPrefix));
  scoped_refptr<DevToolsAgentHost> agent =
      DevToolsAgentHost::GetForId(target_id);
  if (!agent) {
    Send500(connection_id, "No such target id: " + target_id);
    return;
  }

  connection_to_client_[connection_id].reset(new DevToolsAgentHostClientImpl(
      thread_->message_loop(), server_wrapper_.get(), connection_id, agent));

  AcceptWebSocket(connection_id, request);
}
