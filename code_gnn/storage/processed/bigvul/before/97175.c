Frame* WebFrameLoaderClient::dispatchCreatePage() {
  struct WebCore::WindowFeatures features;
  Page* new_page = webframe_->frame()->page()->chrome()->createWindow(
      webframe_->frame(), FrameLoadRequest(), features);

  DCHECK(next_navigation_policy_ != WebKit::WebNavigationPolicyIgnore);
  WebNavigationPolicy policy = next_navigation_policy_;
  next_navigation_policy_ = WebKit::WebNavigationPolicyIgnore;

  if (!new_page)
    return NULL;

  WebViewImpl::FromPage(new_page)->set_initial_navigation_policy(policy);
  return new_page->mainFrame();
}
