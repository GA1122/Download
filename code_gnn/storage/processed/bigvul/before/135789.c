bool SelectionController::HandleTripleClick(
    const MouseEventWithHitTestResults& event) {
  TRACE_EVENT0("blink",
               "SelectionController::handleMousePressEventTripleClick");

  if (!Selection().IsAvailable()) {
    return false;
  }

  if (!mouse_down_allows_multi_click_)
    return HandleSingleClick(event);

  if (event.Event().button != WebPointerProperties::Button::kLeft)
    return false;

  Node* const inner_node = event.InnerNode();
  if (!(inner_node && inner_node->GetLayoutObject() &&
        mouse_down_may_start_select_))
    return false;

  const VisiblePositionInFlatTree& pos =
      VisiblePositionOfHitTestResult(event.GetHitTestResult());
  const VisibleSelectionInFlatTree new_selection =
      pos.IsNotNull() ? CreateVisibleSelectionWithGranularity(
                            SelectionInFlatTree::Builder()
                                .Collapse(pos.ToPositionWithAffinity())
                                .Build(),
                            TextGranularity::kParagraph)
                      : VisibleSelectionInFlatTree();

  const bool is_handle_visible =
      event.Event().FromTouch() && new_selection.IsRange();

  const bool did_select = UpdateSelectionForMouseDownDispatchingSelectStart(
      inner_node,
      ExpandSelectionToRespectUserSelectAll(inner_node,
                                            new_selection.AsSelection()),
      TextGranularity::kParagraph,
      is_handle_visible ? HandleVisibility::kVisible
                        : HandleVisibility::kNotVisible);
  if (!did_select)
    return false;

  if (!Selection().IsHandleVisible())
    return true;
  frame_->GetEventHandler().ShowNonLocatedContextMenu(nullptr,
                                                      kMenuSourceTouch);
  return true;
}
