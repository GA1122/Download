void SelectionController::SelectClosestWordOrLinkFromMouseEvent(
    const MouseEventWithHitTestResults& result) {
  if (!result.GetHitTestResult().IsLiveLink()) {
    SelectClosestWordFromMouseEvent(result);
    return;
  }

  Node* const inner_node = result.InnerNode();

  if (!inner_node || !inner_node->GetLayoutObject() ||
      !mouse_down_may_start_select_)
    return;

  Element* url_element = result.GetHitTestResult().URLElement();
  const VisiblePositionInFlatTree pos =
      VisiblePositionOfHitTestResult(result.GetHitTestResult());
  const SelectionInFlatTree& new_selection =
      pos.IsNotNull() &&
              pos.DeepEquivalent().AnchorNode()->IsDescendantOf(url_element)
          ? SelectionInFlatTree::Builder()
                .SelectAllChildren(*url_element)
                .Build()
          : SelectionInFlatTree();

  UpdateSelectionForMouseDownDispatchingSelectStart(
      inner_node,
      ExpandSelectionToRespectUserSelectAll(inner_node, new_selection),
      TextGranularity::kWord, HandleVisibility::kNotVisible);
}
