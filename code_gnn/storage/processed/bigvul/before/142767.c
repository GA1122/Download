WebMediaPlayer::CorsMode HTMLMediaElement::CorsMode() const {
  const AtomicString& cross_origin_mode = FastGetAttribute(kCrossoriginAttr);
  if (cross_origin_mode.IsNull())
    return WebMediaPlayer::kCorsModeUnspecified;
  if (DeprecatedEqualIgnoringCase(cross_origin_mode, "use-credentials"))
    return WebMediaPlayer::kCorsModeUseCredentials;
  return WebMediaPlayer::kCorsModeAnonymous;
}
