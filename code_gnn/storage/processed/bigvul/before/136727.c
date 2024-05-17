bool FrameSelection::SelectionHasFocus() const {
  GetDocument().UpdateStyleAndLayoutIgnorePendingStylesheets();
  if (ComputeVisibleSelectionInFlatTree().IsNone())
    return false;
  const Node* current =
      ComputeVisibleSelectionInFlatTree().Start().ComputeContainerNode();
  if (!current)
    return false;

  Element* const focused_element = GetDocument().FocusedElement()
                                       ? GetDocument().FocusedElement()
                                       : GetDocument().documentElement();
  if (!focused_element)
    return false;

  if (focused_element->IsTextControl())
    return focused_element->ContainsIncludingHostElements(*current);

  const PositionInFlatTree& focused_position =
      PositionInFlatTree::FirstPositionInNode(*focused_element);
  if (ComputeVisibleSelectionInFlatTree().Start() <= focused_position &&
      ComputeVisibleSelectionInFlatTree().End() >= focused_position)
    return true;

  bool has_editable_style = HasEditableStyle(*current);
  do {
    if (has_editable_style && !HasEditableStyle(*current))
      return false;

    if (current == focused_element)
      return true;
    current = current->ParentOrShadowHostNode();
  } while (current);

  return false;
}
