bool HTMLMediaElement::MediaShouldBeOpaque() const {
  return !IsMediaDataCorsSameOrigin() && ready_state_ < kHaveMetadata &&
         !FastGetAttribute(kSrcAttr).IsEmpty() &&
         EffectivePreloadType() != WebMediaPlayer::kPreloadNone;
}
