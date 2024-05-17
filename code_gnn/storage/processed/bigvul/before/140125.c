bool HTMLMediaElement::stoppedDueToErrors() const {
  if (m_readyState >= kHaveMetadata && m_error) {
    TimeRanges* seekableRanges = seekable();
    if (!seekableRanges->contain(currentTime()))
      return true;
  }

  return false;
}
