bool HTMLMediaElement::endedPlayback(LoopCondition loopCondition) const {
  double dur = duration();
  if (std::isnan(dur))
    return false;


  if (m_readyState < kHaveMetadata)
    return false;

  double now = currentPlaybackPosition();
  if (getDirectionOfPlayback() == Forward)
    return dur > 0 && now >= dur &&
           (loopCondition == LoopCondition::Ignored || !loop());

  DCHECK_EQ(getDirectionOfPlayback(), Backward);
  return now <= earliestPossiblePosition();
}
