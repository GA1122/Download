bool HeadlessDevToolsManagerDelegate::HandleCommand(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    base::DictionaryValue* command) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  if (!browser_)
    return false;

  const base::Value* id_value = command->FindKey("id");
  const base::Value* method_value = command->FindKey("method");
  if (!id_value || !method_value)
    return false;

  const base::DictionaryValue* params = nullptr;
  command->GetDictionary("params", &params);

  const std::string& method = method_value->GetString();
  auto find_it = command_map_.find(method);
  if (find_it == command_map_.end()) {
    find_it = unhandled_command_map_.find(method);
    if (find_it != unhandled_command_map_.end())
      find_it->second.Run(agent_host, session_id, id_value->GetInt(), params);
    return false;
  }

  if (method.find("Browser.") == 0 &&
      agent_host->GetType() != content::DevToolsAgentHost::kTypeBrowser)
    return false;

  auto cmd_result =
      find_it->second.Run(agent_host, session_id, id_value->GetInt(), params);
  if (!cmd_result)
    return false;
  agent_host->SendProtocolMessageToClient(session_id,
                                          ToString(std::move(cmd_result)));
  return true;
}
