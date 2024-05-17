WebKit::WebGeolocationClient* RenderViewImpl::geolocationClient() {
  if (!geolocation_dispatcher_)
    geolocation_dispatcher_ = new GeolocationDispatcher(this);
  return geolocation_dispatcher_;
}