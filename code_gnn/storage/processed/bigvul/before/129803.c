String SystemClipboard::ReadHTML(KURL& url,
                                 unsigned& fragment_start,
                                 unsigned& fragment_end) {
  String html;
  if (IsValidBufferType(buffer_)) {
    clipboard_->ReadHtml(buffer_, &html, &url,
                         static_cast<uint32_t*>(&fragment_start),
                         static_cast<uint32_t*>(&fragment_end));
  }
  if (html.IsEmpty()) {
    url = KURL();
    fragment_start = 0;
    fragment_end = 0;
  }
  return html;
}
