bool OmniboxViewViews::HandleAccessibleAction(
    const ui::AXActionData& action_data) {
  if (read_only())
    return Textfield::HandleAccessibleAction(action_data);

  if (action_data.action == ax::mojom::Action::kSetValue) {
    SetUserText(base::UTF8ToUTF16(action_data.value), true);
    return true;
  } else if (action_data.action == ax::mojom::Action::kReplaceSelectedText) {
    model()->SetInputInProgress(true);
    if (saved_selection_for_focus_change_.IsValid()) {
      SelectRange(saved_selection_for_focus_change_);
      saved_selection_for_focus_change_ = gfx::Range::InvalidRange();
    }
    InsertOrReplaceText(base::UTF8ToUTF16(action_data.value));
    TextChanged();
    return true;
  } else if (action_data.action == ax::mojom::Action::kSetSelection) {
    ui::AXActionData set_selection_action_data;
    set_selection_action_data.action = ax::mojom::Action::kSetSelection;
    set_selection_action_data.anchor_node_id = action_data.anchor_node_id;
    set_selection_action_data.focus_offset =
        action_data.focus_offset - friendly_suggestion_text_prefix_length_;
    set_selection_action_data.anchor_offset =
        action_data.anchor_offset - friendly_suggestion_text_prefix_length_;
    return Textfield::HandleAccessibleAction(set_selection_action_data);
  }

  return Textfield::HandleAccessibleAction(action_data);
}
