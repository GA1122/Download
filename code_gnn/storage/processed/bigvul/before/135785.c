bool SelectionController::HandleMousePressEvent(
    const MouseEventWithHitTestResults& event) {
  TRACE_EVENT0("blink", "SelectionController::handleMousePressEvent");

  mouse_down_may_start_select_ =
      (CanMouseDownStartSelect(event.InnerNode()) || IsLinkSelection(event)) &&
      !event.GetScrollbar();
  mouse_down_was_single_click_in_selection_ = false;
  if (!Selection().IsAvailable()) {
    mouse_down_allows_multi_click_ = !event.Event().FromTouch();
  } else {
    mouse_down_allows_multi_click_ =
        !event.Event().FromTouch() ||
        IsEditablePosition(
            Selection().ComputeVisibleSelectionInDOMTreeDeprecated().Start());
  }

  if (event.Event().click_count >= 3)
    return HandleTripleClick(event);
  if (event.Event().click_count == 2)
    return HandleDoubleClick(event);
  return HandleSingleClick(event);
}
