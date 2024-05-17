HeadlessDevToolsManagerDelegate::GetWindowForTarget(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    int command_id,
    const base::DictionaryValue* params) {
  std::string target_id;
  if (!params->GetString("targetId", &target_id))
    return CreateInvalidParamResponse(command_id, "targetId");

  HeadlessWebContentsImpl* web_contents = HeadlessWebContentsImpl::From(
      browser_->GetWebContentsForDevToolsAgentHostId(target_id));
  if (!web_contents) {
    return CreateErrorResponse(command_id, kErrorServerError,
                               "No web contents for the given target id");
  }

  auto result = base::MakeUnique<base::DictionaryValue>();
  result->SetInteger("windowId", web_contents->window_id());
  result->Set("bounds", CreateBoundsDict(web_contents));
  return CreateSuccessResponse(command_id, std::move(result));
}
