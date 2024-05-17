void HTMLMediaElement::loadResource(const WebMediaPlayerSource& source,
                                    const ContentType& contentType) {
  DCHECK(isMainThread());
  KURL url;
  if (source.isURL()) {
    url = source.getAsURL();
    DCHECK(isSafeToLoadURL(url, Complain));
    BLINK_MEDIA_LOG << "loadResource(" << (void*)this << ", "
                    << urlForLoggingMedia(url) << ", " << contentType.raw()
                    << ")";
  }

  LocalFrame* frame = document().frame();
  if (!frame) {
    mediaLoadingFailed(WebMediaPlayer::NetworkStateFormatError);
    return;
  }

  setNetworkState(kNetworkLoading);

  m_currentSrc = url;

  if (m_audioSourceNode)
    m_audioSourceNode->onCurrentSrcChanged(m_currentSrc);

  BLINK_MEDIA_LOG << "loadResource(" << (void*)this << ") - m_currentSrc -> "
                  << urlForLoggingMedia(m_currentSrc);

  startProgressEventTimer();

  setDisplayMode(Unknown);

  setPlayerPreload();

  if (fastHasAttribute(mutedAttr))
    m_muted = true;

  DCHECK(!m_mediaSource);

  bool attemptLoad = true;

  m_mediaSource = HTMLMediaSource::lookup(url.getString());
  if (m_mediaSource && !m_mediaSource->attachToElement(this)) {
    m_mediaSource = nullptr;
    attemptLoad = false;
  }

  bool canLoadResource = source.isMediaStream() || canLoadURL(url, contentType);
  if (attemptLoad && canLoadResource) {
    DCHECK(!webMediaPlayer());

    if (!source.isMediaStream() && !url.protocolIs("blob") &&
        effectivePreloadType() == WebMediaPlayer::PreloadNone) {
      BLINK_MEDIA_LOG << "loadResource(" << (void*)this
                      << ") : Delaying load because preload == 'none'";
      deferLoad();
    } else {
      startPlayerLoad();
    }
  } else {
    mediaLoadingFailed(WebMediaPlayer::NetworkStateFormatError);
  }

  updateDisplayState();

  if (layoutObject())
    layoutObject()->updateFromElement();
}
