void RenderViewImpl::willPerformClientRedirect(
    WebFrame* frame, const WebURL& from, const WebURL& to, double interval,
    double fire_time) {
  FOR_EACH_OBSERVER(
      RenderViewObserver, observers_,
      WillPerformClientRedirect(frame, from, to, interval, fire_time));
}
