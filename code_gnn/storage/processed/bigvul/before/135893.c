void TextTrack::removeCue(TextTrackCue* cue, ExceptionState& exception_state) {
  DCHECK(cue);



  if (cue->track() != this) {
    exception_state.ThrowDOMException(
        kNotFoundError,
        "The specified cue is not listed in the TextTrack's list of cues.");
    return;
  }

  DCHECK(cues_);

  bool was_removed = cues_->Remove(cue);
  DCHECK(was_removed);

  DCHECK(!cue->IsActive() || GetCueTimeline());

  cue->SetTrack(nullptr);

  if (GetCueTimeline())
    GetCueTimeline()->RemoveCue(this, cue);
}
