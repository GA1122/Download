HeadlessDevToolsManagerDelegate::DisposeBrowserContext(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    int command_id,
    const base::DictionaryValue* params) {
  const base::Value* browser_context_id_value =
      params->FindKey("browserContextId");
  if (!browser_context_id_value)
    return CreateInvalidParamResponse(command_id, "browserContextId");

  HeadlessBrowserContext* context =
      browser_->GetBrowserContextForId(browser_context_id_value->GetString());

  bool success = false;
  if (context && context != browser_->GetDefaultBrowserContext() &&
      context->GetAllWebContents().empty()) {
    success = true;
    context->Close();
  }

  std::unique_ptr<base::Value> result(
      target::DisposeBrowserContextResult::Builder()
          .SetSuccess(success)
          .Build()
          ->Serialize());
  return CreateSuccessResponse(command_id, std::move(result));
}
