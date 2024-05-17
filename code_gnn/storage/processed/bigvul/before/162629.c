HeadlessDevToolsManagerDelegate::GetWindowForTarget(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    int command_id,
    const base::DictionaryValue* params) {
  const base::Value* target_id_value = params->FindKey("targetId");
  if (!target_id_value)
    return CreateInvalidParamResponse(command_id, "targetId");

  HeadlessWebContentsImpl* web_contents = HeadlessWebContentsImpl::From(
      browser_->GetWebContentsForDevToolsAgentHostId(
          target_id_value->GetString()));
  if (!web_contents) {
    return CreateErrorResponse(command_id, kErrorServerError,
                               "No web contents for the given target id");
  }

  auto result = std::make_unique<base::DictionaryValue>();
  result->SetInteger("windowId", web_contents->window_id());
  result->Set("bounds", CreateBoundsDict(web_contents));
  return CreateSuccessResponse(command_id, std::move(result));
}
