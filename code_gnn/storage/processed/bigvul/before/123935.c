void RenderViewImpl::SendUpdateState(const WebHistoryItem& item) {
  if (item.isNull())
    return;

  if (item.urlString() == WebString::fromUTF8(kSwappedOutURL))
    return;

  Send(new ViewHostMsg_UpdateState(
      routing_id_, page_id_, webkit_glue::HistoryItemToString(item)));
}
