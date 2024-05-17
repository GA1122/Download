bool HeadlessDevToolsManagerDelegate::HandleAsyncCommand(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    base::DictionaryValue* command,
    const CommandCallback& callback) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  if (!browser_)
    return false;

  int id;
  std::string method;
  if (!command->GetInteger("id", &id) || !command->GetString("method", &method))
    return false;

  auto find_it = async_command_map_.find(method);
  if (find_it == async_command_map_.end())
    return false;

  const base::DictionaryValue* params = nullptr;
  command->GetDictionary("params", &params);
  find_it->second.Run(agent_host, session_id, id, params, callback);
  return true;
}
