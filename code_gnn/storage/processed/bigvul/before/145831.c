HeadlessDevToolsManagerDelegate::CreateTarget(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    int command_id,
    const base::DictionaryValue* params) {
  std::string url;
  std::string browser_context_id;
  int width = browser_->options()->window_size.width();
  int height = browser_->options()->window_size.height();
  if (!params || !params->GetString("url", &url))
    return CreateInvalidParamResponse(command_id, "url");
  bool enable_begin_frame_control = false;
  params->GetString("browserContextId", &browser_context_id);
  params->GetInteger("width", &width);
  params->GetInteger("height", &height);
  params->GetBoolean("enableBeginFrameControl", &enable_begin_frame_control);

#if defined(OS_MACOSX)
  if (enable_begin_frame_control) {
    return CreateErrorResponse(
        command_id, kErrorServerError,
        "BeginFrameControl is not supported on MacOS yet");
  }
#endif

  HeadlessBrowserContext* context =
      browser_->GetBrowserContextForId(browser_context_id);
  if (!browser_context_id.empty()) {
    context = browser_->GetBrowserContextForId(browser_context_id);
    if (!context)
      return CreateInvalidParamResponse(command_id, "browserContextId");
  } else {
    context = browser_->GetDefaultBrowserContext();
    if (!context) {
      return CreateErrorResponse(command_id, kErrorServerError,
                                 "You specified no |browserContextId|, but "
                                 "there is no default browser context set on "
                                 "HeadlessBrowser");
    }
  }

  HeadlessWebContentsImpl* web_contents_impl = HeadlessWebContentsImpl::From(
      context->CreateWebContentsBuilder()
          .SetInitialURL(GURL(url))
          .SetWindowSize(gfx::Size(width, height))
          .SetEnableBeginFrameControl(enable_begin_frame_control)
          .Build());

  std::unique_ptr<base::Value> result(
      target::CreateTargetResult::Builder()
          .SetTargetId(web_contents_impl->GetDevToolsAgentHostId())
          .Build()
          ->Serialize());
  return CreateSuccessResponse(command_id, std::move(result));
}
