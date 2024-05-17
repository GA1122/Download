double HTMLMediaElement::earliestPossiblePosition() const {
  TimeRanges* seekableRanges = seekable();
  if (seekableRanges && seekableRanges->length() > 0)
    return seekableRanges->start(0, ASSERT_NO_EXCEPTION);

  return currentPlaybackPosition();
}
