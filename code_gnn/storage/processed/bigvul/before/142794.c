void HTMLMediaElement::FinishParsingChildren() {
  HTMLElement::FinishParsingChildren();

  if (Traversal<HTMLTrackElement>::FirstChild(*this))
    ScheduleTextTrackResourceLoad();
}
