void TextTrack::CueDidChange(TextTrackCue* cue, bool update_cue_index) {
  DCHECK(cues_ && cue->track() == this);

  if (update_cue_index)
    cues_->UpdateCueIndex(cue);

  DCHECK(!cue->IsActive());

  if (mode_ == DisabledKeyword())
    return;

  if (GetCueTimeline())
    GetCueTimeline()->AddCue(this, cue);
}
