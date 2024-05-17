static TriState StateUnorderedList(LocalFrame& frame, Event*) {
  return SelectionListState(frame.Selection(), ulTag);
}
