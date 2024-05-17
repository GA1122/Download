bool HTMLMediaElement::IsSafeToLoadURL(const KURL& url,
                                       InvalidURLAction action_if_invalid) {
  if (!url.IsValid()) {
    BLINK_MEDIA_LOG << "isSafeToLoadURL(" << (void*)this << ", "
                    << UrlForLoggingMedia(url)
                    << ") -> FALSE because url is invalid";
    return false;
  }

  LocalFrame* frame = GetDocument().GetFrame();
  if (!frame || !GetDocument().GetSecurityOrigin()->CanDisplay(url)) {
    if (action_if_invalid == kComplain) {
      GetDocument().AddConsoleMessage(ConsoleMessage::Create(
          kSecurityMessageSource, kErrorMessageLevel,
          "Not allowed to load local resource: " + url.ElidedString()));
    }
    BLINK_MEDIA_LOG << "isSafeToLoadURL(" << (void*)this << ", "
                    << UrlForLoggingMedia(url)
                    << ") -> FALSE rejected by SecurityOrigin";
    return false;
  }

  if (!GetDocument().GetContentSecurityPolicy()->AllowMediaFromSource(url)) {
    BLINK_MEDIA_LOG << "isSafeToLoadURL(" << (void*)this << ", "
                    << UrlForLoggingMedia(url)
                    << ") -> rejected by Content Security Policy";
    return false;
  }

  return true;
}
