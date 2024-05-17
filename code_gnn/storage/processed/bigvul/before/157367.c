CueTimeline& HTMLMediaElement::GetCueTimeline() {
  if (!cue_timeline_)
    cue_timeline_ = new CueTimeline(*this);
  return *cue_timeline_;
}
