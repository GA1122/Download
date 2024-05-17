HeadlessDevToolsManagerDelegate::EnableHeadlessExperimental(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    int command_id,
    const base::DictionaryValue* params) {
  content::WebContents* web_contents = agent_host->GetWebContents();
  if (!web_contents) {
    return CreateErrorResponse(command_id, kErrorServerError,
                               "Command not supported on this endpoint");
  }

  HeadlessWebContentsImpl* headless_contents =
      HeadlessWebContentsImpl::From(browser_.get(), web_contents);
  headless_contents->SetBeginFrameEventsEnabled(session_id, true);
  return CreateSuccessResponse(command_id, nullptr);
}
