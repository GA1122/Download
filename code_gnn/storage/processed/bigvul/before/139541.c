static TriState SelectionListState(const FrameSelection& selection,
                                   const QualifiedName& tag_name) {
  if (selection.ComputeVisibleSelectionInDOMTreeDeprecated().IsCaret()) {
    if (EnclosingElementWithTag(
            selection.ComputeVisibleSelectionInDOMTreeDeprecated().Start(),
            tag_name))
      return kTrueTriState;
  } else if (selection.ComputeVisibleSelectionInDOMTreeDeprecated().IsRange()) {
    Element* start_element = EnclosingElementWithTag(
        selection.ComputeVisibleSelectionInDOMTreeDeprecated().Start(),
        tag_name);
    Element* end_element = EnclosingElementWithTag(
        selection.ComputeVisibleSelectionInDOMTreeDeprecated().End(), tag_name);

    if (start_element && end_element && start_element == end_element) {
      if (HasChildTags(*start_element, tag_name.Matches(ulTag) ? olTag : ulTag))
        return kFalseTriState;
      return kTrueTriState;
    }
  }

  return kFalseTriState;
}
