double HTMLMediaElement::effectiveMediaVolume() const {
  if (m_muted)
    return 0;

  return m_volume;
}
