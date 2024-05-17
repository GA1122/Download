HeadlessDevToolsManagerDelegate::CloseTarget(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    int command_id,
    const base::DictionaryValue* params) {
  const base::Value* target_id_value = params->FindKey("targetId");
  if (!target_id_value)
    return CreateInvalidParamResponse(command_id, "targetId");
  HeadlessWebContents* web_contents =
      browser_->GetWebContentsForDevToolsAgentHostId(
          target_id_value->GetString());
  bool success = false;
  if (web_contents) {
    web_contents->Close();
    success = true;
  }
  std::unique_ptr<base::Value> result(target::CloseTargetResult::Builder()
                                          .SetSuccess(success)
                                          .Build()
                                          ->Serialize());
  return CreateSuccessResponse(command_id, std::move(result));
}
