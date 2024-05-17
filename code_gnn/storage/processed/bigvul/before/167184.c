void HTMLMediaElement::TraceWrappers(ScriptWrappableVisitor* visitor) const {
  visitor->TraceWrappers(video_tracks_);
  visitor->TraceWrappers(audio_tracks_);
  visitor->TraceWrappers(text_tracks_);
  HTMLElement::TraceWrappers(visitor);
  Supplementable<HTMLMediaElement>::TraceWrappers(visitor);
}
