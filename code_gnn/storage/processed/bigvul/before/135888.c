void TextTrack::TraceWrappers(const ScriptWrappableVisitor* visitor) const {
  visitor->TraceWrappers(cues_);
  EventTargetWithInlineData::TraceWrappers(visitor);
}
