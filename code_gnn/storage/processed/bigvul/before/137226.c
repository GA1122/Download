bool Textfield::HandleAccessibleAction(const ui::AXActionData& action_data) {
  if (action_data.action == ax::mojom::Action::kSetSelection) {
    if (action_data.anchor_node_id != action_data.focus_node_id)
      return false;
    const gfx::Range range(action_data.anchor_offset, action_data.focus_offset);
    return SetSelectionRange(range);
  }

  if (read_only())
    return View::HandleAccessibleAction(action_data);

  if (action_data.action == ax::mojom::Action::kSetValue) {
    SetText(action_data.value);
    ClearSelection();
    return true;
  } else if (action_data.action == ax::mojom::Action::kReplaceSelectedText) {
    InsertOrReplaceText(action_data.value);
    ClearSelection();
    return true;
  }

  return View::HandleAccessibleAction(action_data);
}
