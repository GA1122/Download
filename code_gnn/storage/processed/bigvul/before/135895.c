void TextTrackCue::CueDidChange(CueMutationAffectsOrder affects_order) {
  if (track_)
    track_->CueDidChange(this, affects_order == kCueMutationAffectsOrder);
}
