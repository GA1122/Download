void InterstitialPageImpl::GetScreenInfo(ScreenInfo* screen_info) {
  WebContentsImpl* web_contents_impl =
      static_cast<WebContentsImpl*>(web_contents_);
  if (!web_contents_impl) {
    WebContentsView::GetDefaultScreenInfo(screen_info);
    return;
  }

  web_contents_impl->GetView()->GetScreenInfo(screen_info);
}
