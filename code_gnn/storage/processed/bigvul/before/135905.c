void TextTrackCue::setId(const AtomicString& id) {
  if (id_ == id)
    return;

  CueWillChange();
  id_ = id;
  CueDidChange();
 }
