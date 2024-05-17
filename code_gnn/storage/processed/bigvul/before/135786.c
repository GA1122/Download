bool SelectionController::HandleMouseReleaseEvent(
    const MouseEventWithHitTestResults& event,
    const LayoutPoint& drag_start_pos) {
  TRACE_EVENT0("blink", "SelectionController::handleMouseReleaseEvent");

  if (!Selection().IsAvailable())
    return false;

  bool handled = false;
  mouse_down_may_start_select_ = false;
  if (mouse_down_was_single_click_in_selection_ &&
      selection_state_ != SelectionState::kExtendedSelection &&
      drag_start_pos == FlooredIntPoint(event.Event().PositionInRootFrame()) &&
      Selection().ComputeVisibleSelectionInDOMTreeDeprecated().IsRange() &&
      event.Event().button != WebPointerProperties::Button::kRight) {
    frame_->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

    SelectionInFlatTree::Builder builder;
    Node* node = event.InnerNode();
    if (node && node->GetLayoutObject() && HasEditableStyle(*node)) {
      const VisiblePositionInFlatTree pos =
          VisiblePositionOfHitTestResult(event.GetHitTestResult());
      if (pos.IsNotNull())
        builder.Collapse(pos.ToPositionWithAffinity());
    }

    if (Selection().ComputeVisibleSelectionInFlatTree() !=
        CreateVisibleSelection(builder.Build())) {
      Selection().SetSelection(ConvertToSelectionInDOMTree(builder.Build()));
    }

    handled = true;
  }

  Selection().NotifyTextControlOfSelectionChange(SetSelectionBy::kUser);

  Selection().SelectFrameElementInParentIfFullySelected();

  if (event.Event().button == WebPointerProperties::Button::kMiddle &&
      !event.IsOverLink()) {
    handled = HandlePasteGlobalSelection(event.Event()) || handled;
  }

  return handled;
}
