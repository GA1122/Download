void RenderFrameImpl::DidReceiveTitle(const blink::WebString& title,
                                      blink::WebTextDirection direction) {
  if (!frame_->Parent()) {
    base::trace_event::TraceLog::GetInstance()->UpdateProcessLabel(
        routing_id_, title.Utf8());

    base::string16 title16 = title.Utf16();
    base::string16 shortened_title = title16.substr(0, kMaxTitleChars);
    Send(new FrameHostMsg_UpdateTitle(routing_id_,
                                      shortened_title, direction));
  } else {
    GURL loading_url = GetLoadingUrl();
    if (!loading_url.host().empty() &&
        loading_url.scheme() != url::kFileScheme) {
      std::string subframe_title = "Subframe: " + loading_url.scheme() +
                                   url::kStandardSchemeSeparator +
                                   loading_url.host();
      base::trace_event::TraceLog::GetInstance()->UpdateProcessLabel(
          routing_id_, subframe_title);
    }
  }

  UpdateEncoding(frame_, frame_->View()->PageEncoding().Utf8());
}
