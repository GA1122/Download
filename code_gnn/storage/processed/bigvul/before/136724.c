void FrameSelection::SelectSubString(const Element& element,
                                     int offset,
                                     int length) {
  VisiblePosition start = VisiblePosition::FirstPositionInNode(element);
  for (int i = 0; i < offset; ++i)
    start = NextPositionOf(start);
  if (start.IsNull())
    return;

  VisiblePosition end(start);
  for (int i = 0; i < length; ++i)
    end = NextPositionOf(end);
  if (end.IsNull())
    return;

  SetSelectionAndEndTyping(
      SelectionInDOMTree::Builder()
          .SetBaseAndExtent(start.DeepEquivalent(), end.DeepEquivalent())
          .SetAffinity(start.Affinity())
          .Build());
}
