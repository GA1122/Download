void RenderViewImpl::UpdateSessionHistory(WebFrame* frame) {
  if (page_id_ == -1)
    return;

  const WebHistoryItem& item =
      webview()->mainFrame()->previousHistoryItem();
  SendUpdateState(item);
}
