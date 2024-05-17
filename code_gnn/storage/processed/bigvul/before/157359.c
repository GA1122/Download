WebMediaPlayer::CORSMode HTMLMediaElement::CorsMode() const {
  const AtomicString& cross_origin_mode = FastGetAttribute(crossoriginAttr);
  if (cross_origin_mode.IsNull())
    return WebMediaPlayer::kCORSModeUnspecified;
  if (DeprecatedEqualIgnoringCase(cross_origin_mode, "use-credentials"))
    return WebMediaPlayer::kCORSModeUseCredentials;
  return WebMediaPlayer::kCORSModeAnonymous;
}
