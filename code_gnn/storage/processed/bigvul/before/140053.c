bool HTMLMediaElement::potentiallyPlaying() const {
  bool pausedToBuffer =
      m_readyStateMaximum >= kHaveFutureData && m_readyState < kHaveFutureData;
  return (pausedToBuffer || m_readyState >= kHaveFutureData) &&
         couldPlayIfEnoughData();
}
