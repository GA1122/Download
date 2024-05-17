void TextTrack::Trace(blink::Visitor* visitor) {
  visitor->Trace(cues_);
  visitor->Trace(active_cues_);
  visitor->Trace(track_list_);
  TrackBase::Trace(visitor);
  EventTargetWithInlineData::Trace(visitor);
}
