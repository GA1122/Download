void TextTrackCueList::TraceWrappers(
    const ScriptWrappableVisitor* visitor) const {
  for (auto cue : list_) {
    visitor->TraceWrappers(cue);
  }
}
