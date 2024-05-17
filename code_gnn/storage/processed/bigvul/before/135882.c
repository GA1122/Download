void TextTrack::RemoveAllCues() {
  if (!cues_)
    return;

  if (GetCueTimeline())
    GetCueTimeline()->RemoveCues(this, cues_.Get());

  for (size_t i = 0; i < cues_->length(); ++i)
    cues_->AnonymousIndexedGetter(i)->SetTrack(nullptr);

  cues_->RemoveAll();
  if (active_cues_)
    active_cues_->RemoveAll();
}
