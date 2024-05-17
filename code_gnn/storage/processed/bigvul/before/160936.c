ChromeClientImpl::ChromeClientImpl(WebViewImpl* web_view)
    : web_view_(web_view),
      cursor_overridden_(false),
      did_request_non_empty_tool_tip_(false) {}
