WebMediaPlayer::CORSMode HTMLMediaElement::corsMode() const {
  const AtomicString& crossOriginMode = fastGetAttribute(crossoriginAttr);
  if (crossOriginMode.isNull())
    return WebMediaPlayer::CORSModeUnspecified;
  if (equalIgnoringCase(crossOriginMode, "use-credentials"))
    return WebMediaPlayer::CORSModeUseCredentials;
  return WebMediaPlayer::CORSModeAnonymous;
}
