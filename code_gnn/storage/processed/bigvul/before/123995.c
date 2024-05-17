void RenderViewImpl::willPerformClientRedirect(
    WebFrame* frame, const WebURL& from, const WebURL& to, double interval,
    double fire_time) {
  const WebURL& blank_url = GURL(chrome::kAboutBlankURL);

  FOR_EACH_OBSERVER(
      RenderViewObserver, observers_,
      WillPerformClientRedirect(frame,
                                from == GURL(kSwappedOutURL) ? blank_url : from,
                                to, interval, fire_time));
}
