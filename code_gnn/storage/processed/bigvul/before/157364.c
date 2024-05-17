bool HTMLMediaElement::EndedPlayback(LoopCondition loop_condition) const {
  double dur = duration();
  if (std::isnan(dur))
    return false;


  if (ready_state_ < kHaveMetadata)
    return false;

  double now = CurrentPlaybackPosition();

  UMA_HISTOGRAM_BOOLEAN("Media.MediaElement.PlaybackPositionIsInfinity",
                        now == std::numeric_limits<double>::infinity());
  if (dur == std::numeric_limits<double>::infinity())
    return false;

  if (GetDirectionOfPlayback() == kForward) {
    return dur > 0 && now >= dur &&
           (loop_condition == LoopCondition::kIgnored || !Loop());
  }

  DCHECK_EQ(GetDirectionOfPlayback(), kBackward);
  return now <= EarliestPossiblePosition();
}
