bool HTMLMediaElement::StoppedDueToErrors() const {
  if (ready_state_ >= kHaveMetadata && error_) {
    TimeRanges* seekable_ranges = seekable();
    if (!seekable_ranges->Contain(currentTime()))
      return true;
  }

  return false;
}
