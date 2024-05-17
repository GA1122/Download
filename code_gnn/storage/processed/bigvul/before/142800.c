CueTimeline& HTMLMediaElement::GetCueTimeline() {
  if (!cue_timeline_)
    cue_timeline_ = MakeGarbageCollected<CueTimeline>(*this);
  return *cue_timeline_;
}
