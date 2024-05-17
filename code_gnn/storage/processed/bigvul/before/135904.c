void TextTrackCue::Trace(blink::Visitor* visitor) {
  visitor->Trace(track_);
  EventTargetWithInlineData::Trace(visitor);
}
