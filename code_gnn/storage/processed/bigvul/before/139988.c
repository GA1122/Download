void HTMLMediaElement::finishParsingChildren() {
  HTMLElement::finishParsingChildren();

  if (Traversal<HTMLTrackElement>::firstChild(*this))
    scheduleTextTrackResourceLoad();
}
