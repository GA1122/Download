void AutofillDialogViews::UpdateDetailsGroupState(const DetailsGroup& group) {
  const SuggestionState& suggestion_state =
      delegate_->SuggestionStateForSection(group.section);
  group.suggested_info->SetState(suggestion_state);
  group.manual_input->SetVisible(!suggestion_state.visible);

  UpdateButtonStripExtraView();

  const bool has_menu = !!delegate_->MenuModelForSection(group.section);

  if (group.suggested_button)
    group.suggested_button->SetVisible(has_menu);

  if (group.container) {
    group.container->SetForwardMouseEvents(
        has_menu && suggestion_state.visible);
    group.container->SetVisible(delegate_->SectionIsActive(group.section));
    if (group.container->visible())
      ValidateGroup(group, VALIDATE_EDIT);
  }

  ContentsPreferredSizeChanged();
}
