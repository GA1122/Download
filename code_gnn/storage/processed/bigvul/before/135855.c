void CueTimeline::EndIgnoringUpdateRequests() {
  DCHECK(ignore_update_);
  --ignore_update_;
  if (!ignore_update_)
    UpdateActiveCues(MediaElement().currentTime());
}
