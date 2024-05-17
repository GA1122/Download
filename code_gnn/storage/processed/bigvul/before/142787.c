double HTMLMediaElement::EffectiveMediaVolume() const {
  if (muted_)
    return 0;

  return volume_;
}
