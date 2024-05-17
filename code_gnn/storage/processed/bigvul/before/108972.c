void RenderViewImpl::OnSetCompositionFromExistingText(
    int start, int end,
    const std::vector<WebKit::WebCompositionUnderline>& underlines) {
  if (!webview())
    return;
  webview()->setCompositionFromExistingText(start, end, underlines);
}
