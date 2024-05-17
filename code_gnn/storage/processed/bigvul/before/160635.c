void RenderFrameImpl::LoadNavigationErrorPage(
    const WebURLRequest& failed_request,
    const WebURLError& error,
    bool replace,
    HistoryEntry* entry,
    const base::Optional<std::string>& error_page_content) {
  blink::WebFrameLoadType frame_load_type =
      entry ? blink::WebFrameLoadType::kBackForward
            : blink::WebFrameLoadType::kStandard;
  const blink::WebHistoryItem& history_item =
      entry ? entry->root() : blink::WebHistoryItem();

  if (error.reason() == net::ERR_BLOCKED_BY_RESPONSE) {
    const blink::WebHistoryItem& blank_history_item = blink::WebHistoryItem();
    frame_load_type = blink::WebFrameLoadType::kStandard;
    LoadNavigationErrorPageInternal("", GURL("data:,"), WebURL(), replace,
                                    frame_load_type, blank_history_item);
    return;
  }

  std::string error_html;
  if (error_page_content.has_value()) {
    error_html = error_page_content.value();
    GetContentClient()->renderer()->PrepareErrorPage(this, failed_request,
                                                     error, nullptr, nullptr);
  } else {
    GetContentClient()->renderer()->PrepareErrorPage(
        this, failed_request, error, &error_html, nullptr);
  }
  LoadNavigationErrorPageInternal(error_html, GURL(kUnreachableWebDataURL),
                                  error.url(), replace, frame_load_type,
                                  history_item);
}
