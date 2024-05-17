WebViewBase* WebLocalFrameImpl::ViewImpl() const {
  if (!GetFrame())
    return nullptr;
  return GetFrame()->GetPage()->GetChromeClient().GetWebView();
}
