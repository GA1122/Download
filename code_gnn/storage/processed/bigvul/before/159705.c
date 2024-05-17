void DevToolsUIBindings::DispatchProtocolMessage(
    content::DevToolsAgentHost* agent_host, const std::string& message) {
  DCHECK(agent_host == agent_host_.get());
  if (!frontend_host_ || reloading_)
    return;

  if (message.length() < kMaxMessageChunkSize) {
    std::string param;
    base::EscapeJSONString(message, true, &param);
    base::string16 javascript =
        base::UTF8ToUTF16("DevToolsAPI.dispatchMessage(" + param + ");");
    web_contents_->GetMainFrame()->ExecuteJavaScript(javascript);
    return;
  }

  base::Value total_size(static_cast<int>(message.length()));
  for (size_t pos = 0; pos < message.length(); pos += kMaxMessageChunkSize) {
    base::Value message_value(message.substr(pos, kMaxMessageChunkSize));
    CallClientFunction("DevToolsAPI.dispatchMessageChunk",
                       &message_value, pos ? NULL : &total_size, NULL);
  }
}
