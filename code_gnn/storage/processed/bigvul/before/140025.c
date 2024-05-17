void HTMLMediaElement::loadSourceFromAttribute() {
  m_loadState = LoadingFromSrcAttr;
  const AtomicString& srcValue = fastGetAttribute(srcAttr);

  if (srcValue.isEmpty()) {
    mediaLoadingFailed(WebMediaPlayer::NetworkStateFormatError);
    BLINK_MEDIA_LOG << "loadSourceFromAttribute(" << (void*)this
                    << "), empty 'src'";
    return;
  }

  KURL mediaURL = document().completeURL(srcValue);
  if (!isSafeToLoadURL(mediaURL, Complain)) {
    mediaLoadingFailed(WebMediaPlayer::NetworkStateFormatError);
    return;
  }

  loadResource(WebMediaPlayerSource(WebURL(mediaURL)), ContentType((String())));
}
