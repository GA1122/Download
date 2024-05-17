bool Editor::FindString(const String& target, FindOptions options) {
  VisibleSelection selection =
      GetFrame().Selection().ComputeVisibleSelectionInDOMTreeDeprecated();

  Range* result_range = FindRangeOfString(
      target, EphemeralRange(selection.Start(), selection.End()),
      static_cast<FindOptions>(options | kFindAPICall));

  if (!result_range)
    return false;

  GetFrame().Selection().SetSelection(
      SelectionInDOMTree::Builder()
          .SetBaseAndExtent(EphemeralRange(result_range))
          .Build());
  GetFrame().Selection().RevealSelection();
  return true;
}
