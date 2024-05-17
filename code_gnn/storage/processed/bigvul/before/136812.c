MediaQueryList* LocalDOMWindow::matchMedia(const String& media) {
  return document() ? document()->GetMediaQueryMatcher().MatchMedia(media)
                    : nullptr;
}
