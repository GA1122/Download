void RenderFrameImpl::LoadNavigationErrorPageInternal(
    const std::string& error_html,
    const GURL& error_page_url,
    const GURL& error_url,
    bool replace,
    blink::WebFrameLoadType frame_load_type,
    const blink::WebHistoryItem& history_item) {
  frame_->LoadData(error_html, WebString::FromUTF8("text/html"),
                   WebString::FromUTF8("UTF-8"), error_page_url, error_url,
                   replace, frame_load_type, history_item,
                   blink::kWebHistoryDifferentDocumentLoad, false);
}
