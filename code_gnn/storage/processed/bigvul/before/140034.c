void HTMLMediaElement::mediaSourceOpened(WebMediaSource* webMediaSource) {
  setShouldDelayLoadEvent(false);
  m_mediaSource->setWebMediaSourceAndOpen(WTF::wrapUnique(webMediaSource));
}
