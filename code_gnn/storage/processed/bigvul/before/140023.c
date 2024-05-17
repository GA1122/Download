void HTMLMediaElement::loadNextSourceChild() {
  ContentType contentType((String()));
  KURL mediaURL = selectNextSourceChild(&contentType, Complain);
  if (!mediaURL.isValid()) {
    waitForSourceChange();
    return;
  }

  resetMediaPlayerAndMediaSource();

  m_loadState = LoadingFromSourceElement;
  loadResource(WebMediaPlayerSource(WebURL(mediaURL)), contentType);
}
