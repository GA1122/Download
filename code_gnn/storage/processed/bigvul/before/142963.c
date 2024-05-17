TimeRanges* HTMLMediaElement::played() {
  if (playing_) {
    double time = currentTime();
    if (time > last_seek_time_)
      AddPlayedRange(last_seek_time_, time);
  }

  if (!played_time_ranges_)
    played_time_ranges_ = TimeRanges::Create();

  return played_time_ranges_->Copy();
}
