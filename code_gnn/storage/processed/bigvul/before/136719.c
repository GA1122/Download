void FrameSelection::MoveRangeSelectionInternal(
    const SelectionInDOMTree& new_selection,
    TextGranularity granularity) {
  if (new_selection.IsNone())
    return;

  const VisibleSelection& visible_selection =
      CreateVisibleSelectionWithGranularity(new_selection, granularity);
  if (visible_selection.IsNone())
    return;

  SelectionInDOMTree::Builder builder;
  if (visible_selection.IsBaseFirst()) {
    builder.SetBaseAndExtent(visible_selection.Start(),
                             visible_selection.End());
  } else {
    builder.SetBaseAndExtent(visible_selection.End(),
                             visible_selection.Start());
  }
  builder.SetAffinity(visible_selection.Affinity());
  SetSelection(builder.Build(), SetSelectionOptions::Builder()
                                    .SetShouldCloseTyping(true)
                                    .SetShouldClearTypingStyle(true)
                                    .SetGranularity(granularity)
                                    .SetShouldShowHandle(IsHandleVisible())
                                    .Build());
}
