static bool ExpandSelectionToGranularity(LocalFrame& frame,
                                         TextGranularity granularity) {
  const VisibleSelection& selection = CreateVisibleSelectionWithGranularity(
      SelectionInDOMTree::Builder()
          .SetBaseAndExtent(frame.Selection()
                                .ComputeVisibleSelectionInDOMTreeDeprecated()
                                .Base(),
                            frame.Selection()
                                .ComputeVisibleSelectionInDOMTreeDeprecated()
                                .Extent())
          .Build(),
      granularity);
  const EphemeralRange new_range = selection.ToNormalizedEphemeralRange();
  if (new_range.IsNull())
    return false;
  if (new_range.IsCollapsed())
    return false;
  frame.Selection().SetSelection(
      SelectionInDOMTree::Builder().SetBaseAndExtent(new_range).Build(),
      FrameSelection::kCloseTyping);
  return true;
}
