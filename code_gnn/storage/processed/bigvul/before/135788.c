bool SelectionController::HandleSingleClick(
    const MouseEventWithHitTestResults& event) {
  TRACE_EVENT0("blink",
               "SelectionController::handleMousePressEventSingleClick");

  DCHECK(!frame_->GetDocument()->NeedsLayoutTreeUpdate());
  Node* inner_node = event.InnerNode();
  if (!(inner_node && inner_node->GetLayoutObject() &&
        mouse_down_may_start_select_))
    return false;

  bool extend_selection = IsExtendingSelection(event);

  const VisiblePositionInFlatTree& visible_hit_pos =
      VisiblePositionOfHitTestResult(event.GetHitTestResult());
  const VisiblePositionInFlatTree& visible_pos =
      visible_hit_pos.IsNull()
          ? CreateVisiblePosition(
                PositionInFlatTree::FirstPositionInOrBeforeNode(inner_node))
          : visible_hit_pos;
  const VisibleSelectionInFlatTree& selection =
      this->Selection().ComputeVisibleSelectionInFlatTree();

  if (LocalFrameView* view = frame_->View()) {
    const LayoutPoint v_point = view->RootFrameToContents(
        FlooredIntPoint(event.Event().PositionInRootFrame()));
    if (!extend_selection && this->Selection().Contains(v_point)) {
      mouse_down_was_single_click_in_selection_ = true;
      if (!event.Event().FromTouch())
        return false;

      if (!this->Selection().IsHandleVisible()) {
        const bool did_select =
            UpdateSelectionForMouseDownDispatchingSelectStart(
                inner_node, selection.AsSelection(),
                TextGranularity::kCharacter, HandleVisibility::kVisible);
        if (did_select) {
          frame_->GetEventHandler().ShowNonLocatedContextMenu(nullptr,
                                                              kMenuSourceTouch);
        }
        return false;
      }
    }
  }

  if (extend_selection && !selection.IsNone()) {
    const PositionInFlatTree& pos = AdjustPositionRespectUserSelectAll(
        inner_node, selection.Start(), selection.End(),
        visible_pos.DeepEquivalent());
    const TextGranularity granularity = Selection().Granularity();
    if (pos.IsNull()) {
      UpdateSelectionForMouseDownDispatchingSelectStart(
          inner_node, selection.AsSelection(), granularity,
          HandleVisibility::kNotVisible);
      return false;
    }
    UpdateSelectionForMouseDownDispatchingSelectStart(
        inner_node,
        frame_->GetEditor().Behavior().ShouldConsiderSelectionAsDirectional()
            ? ExtendSelectionAsDirectional(pos, selection, granularity)
            : ExtendSelectionAsNonDirectional(pos, selection, granularity),
        granularity, HandleVisibility::kNotVisible);
    return false;
  }

  if (selection_state_ == SelectionState::kExtendedSelection) {
    UpdateSelectionForMouseDownDispatchingSelectStart(
        inner_node, selection.AsSelection(), TextGranularity::kCharacter,
        HandleVisibility::kNotVisible);
    return false;
  }

  if (visible_pos.IsNull()) {
    UpdateSelectionForMouseDownDispatchingSelectStart(
        inner_node, SelectionInFlatTree(), TextGranularity::kCharacter,
        HandleVisibility::kNotVisible);
    return false;
  }

  bool is_handle_visible = false;
  const bool has_editable_style = HasEditableStyle(*inner_node);
  if (has_editable_style) {
    const bool is_text_box_empty =
        !RootEditableElement(*inner_node)->HasChildren();
    const bool not_left_click =
        event.Event().button != WebPointerProperties::Button::kLeft;
    if (!is_text_box_empty || not_left_click)
      is_handle_visible = event.Event().FromTouch();
  }

  UpdateSelectionForMouseDownDispatchingSelectStart(
      inner_node,
      ExpandSelectionToRespectUserSelectAll(
          inner_node, SelectionInFlatTree::Builder()
                          .Collapse(visible_pos.ToPositionWithAffinity())
                          .Build()),
      TextGranularity::kCharacter,
      is_handle_visible ? HandleVisibility::kVisible
                        : HandleVisibility::kNotVisible);

  if (has_editable_style && event.Event().FromTouch()) {
    frame_->GetTextSuggestionController().HandlePotentialMisspelledWordTap(
        visible_pos.DeepEquivalent());
  }

  return false;
}
