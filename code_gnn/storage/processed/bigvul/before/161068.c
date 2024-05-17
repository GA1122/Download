void DevToolsHttpHandler::OnJsonRequest(
    int connection_id,
    const net::HttpServerRequestInfo& info) {
  std::string path = info.path.substr(5);

  std::string query;
  size_t query_pos = path.find("?");
  if (query_pos != std::string::npos) {
    query = path.substr(query_pos + 1);
    path = path.substr(0, query_pos);
  }

  size_t fragment_pos = path.find("#");
  if (fragment_pos != std::string::npos)
    path = path.substr(0, fragment_pos);

  std::string command;
  std::string target_id;
  if (!ParseJsonPath(path, &command, &target_id)) {
    SendJson(connection_id, net::HTTP_NOT_FOUND, nullptr,
             "Malformed query: " + info.path);
    return;
  }

  if (command == "version") {
    base::DictionaryValue version;
    version.SetString("Protocol-Version",
                      DevToolsAgentHost::GetProtocolVersion());
    version.SetString("WebKit-Version", GetWebKitVersion());
    version.SetString("Browser", GetContentClient()->GetProduct());
    version.SetString("User-Agent", GetContentClient()->GetUserAgent());
    version.SetString("V8-Version", V8_VERSION_STRING);
    std::string host = info.headers["host"];
    version.SetString(
        kTargetWebSocketDebuggerUrlField,
        base::StringPrintf("ws://%s%s", host.c_str(), browser_guid_.c_str()));
#if defined(OS_ANDROID)
    version.SetString(
        "Android-Package",
        base::android::BuildInfo::GetInstance()->host_package_name());
#endif
    SendJson(connection_id, net::HTTP_OK, &version, std::string());
    return;
  }

  if (command == "protocol") {
    DecompressAndSendJsonProtocol(connection_id);
    return;
  }

  if (command == "list") {
    DevToolsManager* manager = DevToolsManager::GetInstance();
    DevToolsAgentHost::List list =
        manager->delegate() ? manager->delegate()->RemoteDebuggingTargets()
                            : DevToolsAgentHost::GetOrCreateAll();
    RespondToJsonList(connection_id, info.headers["host"], std::move(list));
    return;
  }

  if (command == "new") {
    GURL url(net::UnescapeURLComponent(
        query, net::UnescapeRule::URL_SPECIAL_CHARS_EXCEPT_PATH_SEPARATORS |
                   net::UnescapeRule::PATH_SEPARATORS));
    if (!url.is_valid())
      url = GURL(url::kAboutBlankURL);
    scoped_refptr<DevToolsAgentHost> agent_host = nullptr;
    agent_host = delegate_->CreateNewTarget(url);
    if (!agent_host) {
      SendJson(connection_id, net::HTTP_INTERNAL_SERVER_ERROR, nullptr,
               "Could not create new page");
      return;
    }
    std::string host = info.headers["host"];
    std::unique_ptr<base::DictionaryValue> dictionary(
        SerializeDescriptor(agent_host, host));
    SendJson(connection_id, net::HTTP_OK, dictionary.get(), std::string());
    return;
  }

  if (command == "activate" || command == "close") {
    scoped_refptr<DevToolsAgentHost> agent_host =
        DevToolsAgentHost::GetForId(target_id);
    if (!agent_host) {
      SendJson(connection_id, net::HTTP_NOT_FOUND, nullptr,
               "No such target id: " + target_id);
      return;
    }

    if (command == "activate") {
      if (agent_host->Activate()) {
        SendJson(connection_id, net::HTTP_OK, nullptr, "Target activated");
      } else {
        SendJson(connection_id, net::HTTP_INTERNAL_SERVER_ERROR, nullptr,
                 "Could not activate target id: " + target_id);
      }
      return;
    }

    if (command == "close") {
      if (agent_host->Close()) {
        SendJson(connection_id, net::HTTP_OK, nullptr, "Target is closing");
      } else {
        SendJson(connection_id, net::HTTP_INTERNAL_SERVER_ERROR, nullptr,
                 "Could not close target id: " + target_id);
      }
      return;
    }
  }
  SendJson(connection_id, net::HTTP_NOT_FOUND, nullptr,
           "Unknown command: " + command);
  return;
}
