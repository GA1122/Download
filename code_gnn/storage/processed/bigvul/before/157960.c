void RenderViewImpl::OnSetPageScale(float page_scale_factor) {
  if (!webview())
    return;
  webview()->SetPageScaleFactor(page_scale_factor);
}
