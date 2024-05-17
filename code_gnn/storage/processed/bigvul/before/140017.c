bool HTMLMediaElement::isSafeToLoadURL(const KURL& url,
                                       InvalidURLAction actionIfInvalid) {
  if (!url.isValid()) {
    BLINK_MEDIA_LOG << "isSafeToLoadURL(" << (void*)this << ", "
                    << urlForLoggingMedia(url)
                    << ") -> FALSE because url is invalid";
    return false;
  }

  LocalFrame* frame = document().frame();
  if (!frame || !document().getSecurityOrigin()->canDisplay(url)) {
    if (actionIfInvalid == Complain)
      FrameLoader::reportLocalLoadFailed(frame, url.elidedString());
    BLINK_MEDIA_LOG << "isSafeToLoadURL(" << (void*)this << ", "
                    << urlForLoggingMedia(url)
                    << ") -> FALSE rejected by SecurityOrigin";
    return false;
  }

  if (!document().contentSecurityPolicy()->allowMediaFromSource(url)) {
    BLINK_MEDIA_LOG << "isSafeToLoadURL(" << (void*)this << ", "
                    << urlForLoggingMedia(url)
                    << ") -> rejected by Content Security Policy";
    return false;
  }

  return true;
}
