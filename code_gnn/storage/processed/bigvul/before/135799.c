bool SelectionController::SelectClosestWordFromHitTestResult(
    const HitTestResult& result,
    AppendTrailingWhitespace append_trailing_whitespace,
    SelectInputEventType select_input_event_type) {
  Node* const inner_node = result.InnerNode();

  if (!inner_node || !inner_node->GetLayoutObject() ||
      !inner_node->GetLayoutObject()->IsSelectable())
    return false;

  HitTestResult adjusted_hit_test_result = result;
  if (select_input_event_type == SelectInputEventType::kTouch &&
      result.GetImage())
    adjusted_hit_test_result.SetNodeAndPosition(result.InnerNode(),
                                                LayoutPoint(0, 0));

  const VisiblePositionInFlatTree& pos =
      VisiblePositionOfHitTestResult(adjusted_hit_test_result);
  const VisibleSelectionInFlatTree& new_selection =
      pos.IsNotNull() ? CreateVisibleSelectionWithGranularity(
                            SelectionInFlatTree::Builder()
                                .Collapse(pos.ToPositionWithAffinity())
                                .Build(),
                            TextGranularity::kWord)
                      : VisibleSelectionInFlatTree();

  HandleVisibility visibility = HandleVisibility::kNotVisible;
  if (select_input_event_type == SelectInputEventType::kTouch) {
    EphemeralRangeInFlatTree range(new_selection.Start(), new_selection.End());
    const String& str = PlainText(
        range,
        TextIteratorBehavior::Builder()
            .SetEmitsObjectReplacementCharacter(HasEditableStyle(*inner_node))
            .Build());
    if (str.IsEmpty() || str.SimplifyWhiteSpace().ContainsOnlyWhitespace())
      return false;

    if (new_selection.RootEditableElement() &&
        pos.DeepEquivalent() == VisiblePositionInFlatTree::LastPositionInNode(
                                    *new_selection.RootEditableElement())
                                    .DeepEquivalent())
      return false;

    visibility = HandleVisibility::kVisible;
  }

  const SelectionInFlatTree& adjusted_selection =
      append_trailing_whitespace == AppendTrailingWhitespace::kShouldAppend
          ? AdjustSelectionWithTrailingWhitespace(new_selection.AsSelection())
          : new_selection.AsSelection();

  return UpdateSelectionForMouseDownDispatchingSelectStart(
      inner_node,
      ExpandSelectionToRespectUserSelectAll(inner_node, adjusted_selection),
      TextGranularity::kWord, visibility);
}
