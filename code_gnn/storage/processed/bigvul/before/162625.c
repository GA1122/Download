HeadlessDevToolsManagerDelegate::CreateTarget(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    int command_id,
    const base::DictionaryValue* params) {
  std::string url;

  if (const base::Value* url_value = params->FindKey("url")) {
    url = url_value->GetString();
  } else {
    return CreateInvalidParamResponse(command_id, "url");
  }

  std::string browser_context_id;
  if (const base::Value* browser_context_id_value =
          params->FindKey("browserContextId")) {
    browser_context_id = browser_context_id_value->GetString();
  }

  int width = browser_->options()->window_size.width();
  if (const base::Value* width_value = params->FindKey("width"))
    width = width_value->GetInt();

  int height = browser_->options()->window_size.height();
  if (const base::Value* height_value = params->FindKey("height"))
    height = height_value->GetInt();

  bool enable_begin_frame_control = false;
  if (const base::Value* enable_begin_frame_control_value =
          params->FindKey("enableBeginFrameControl")) {
    enable_begin_frame_control = enable_begin_frame_control_value->GetBool();
  }

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
