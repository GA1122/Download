void HTMLMediaElement::startPlayerLoad(const KURL& playerProvidedUrl) {
  DCHECK(!m_webMediaPlayer);

  WebMediaPlayerSource source;
  if (m_srcObject) {
    source = WebMediaPlayerSource(WebMediaStream(m_srcObject));
  } else {
    KURL requestURL =
        playerProvidedUrl.isNull() ? KURL(m_currentSrc) : playerProvidedUrl;
    if (!requestURL.user().isEmpty())
      requestURL.setUser(String());
    if (!requestURL.pass().isEmpty())
      requestURL.setPass(String());

    KURL kurl(ParsedURLString, requestURL);
    source = WebMediaPlayerSource(WebURL(kurl));
  }

  LocalFrame* frame = document().frame();
  if (!frame) {
    mediaLoadingFailed(WebMediaPlayer::NetworkStateFormatError);
    return;
  }

  m_webMediaPlayer =
      frame->loader().client()->createWebMediaPlayer(*this, source, this);
  if (!m_webMediaPlayer) {
    mediaLoadingFailed(WebMediaPlayer::NetworkStateFormatError);
    return;
  }

  if (layoutObject())
    layoutObject()->setShouldDoFullPaintInvalidation();
  m_audioSourceProvider.wrap(m_webMediaPlayer->getAudioSourceProvider());
  m_webMediaPlayer->setVolume(effectiveMediaVolume());

  m_webMediaPlayer->setPoster(posterImageURL());

  m_webMediaPlayer->setPreload(effectivePreloadType());

  m_webMediaPlayer->requestRemotePlaybackDisabled(
      fastHasAttribute(disableremoteplaybackAttr));

  m_webMediaPlayer->load(loadType(), source, corsMode());

  if (isFullscreen())
    m_webMediaPlayer->enteredFullscreen();

  m_webMediaPlayer->becameDominantVisibleContent(m_mostlyFillingViewport);
}
