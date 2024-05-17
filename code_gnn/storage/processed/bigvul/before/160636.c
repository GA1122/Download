void RenderFrameImpl::LoadNavigationErrorPageForHttpStatusError(
    const WebURLRequest& failed_request,
    const GURL& unreachable_url,
    int http_status,
    bool replace,
    HistoryEntry* entry) {
  blink::WebFrameLoadType frame_load_type =
      entry ? blink::WebFrameLoadType::kBackForward
            : blink::WebFrameLoadType::kStandard;
  const blink::WebHistoryItem& history_item =
      entry ? entry->root() : blink::WebHistoryItem();

  std::string error_html;
  GetContentClient()->renderer()->PrepareErrorPageForHttpStatusError(
      this, failed_request, unreachable_url, http_status, &error_html, nullptr);
  LoadNavigationErrorPageInternal(error_html, GURL(kUnreachableWebDataURL),
                                  unreachable_url, replace, frame_load_type,
                                  history_item);
}
