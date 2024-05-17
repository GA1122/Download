blink::WebAppBannerClient* RenderFrameImpl::appBannerClient() {
  if (!app_banner_client_) {
    app_banner_client_ =
        GetContentClient()->renderer()->CreateAppBannerClient(this);
  }

  return app_banner_client_.get();
}
