void FrameSelection::MoveRangeSelectionExtent(const IntPoint& contents_point) {
  if (ComputeVisibleSelectionInDOMTree().IsNone())
    return;

  SetSelection(
      SelectionInDOMTree::Builder(
          GetGranularityStrategy()->UpdateExtent(contents_point, frame_))
          .Build(),
      SetSelectionOptions::Builder()
          .SetShouldCloseTyping(true)
          .SetShouldClearTypingStyle(true)
          .SetDoNotClearStrategy(true)
          .SetSetSelectionBy(SetSelectionBy::kUser)
          .SetShouldShowHandle(true)
          .Build());
}
