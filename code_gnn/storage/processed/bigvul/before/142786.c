double HTMLMediaElement::EarliestPossiblePosition() const {
  TimeRanges* seekable_ranges = seekable();
  if (seekable_ranges && seekable_ranges->length() > 0)
    return seekable_ranges->start(0, ASSERT_NO_EXCEPTION);

  return CurrentPlaybackPosition();
}
