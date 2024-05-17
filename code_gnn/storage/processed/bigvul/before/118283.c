void AutofillDialogViews::OnPerformAction(views::Combobox* combobox) {
  DialogSection section = GroupForView(combobox)->section;
  InputEditedOrActivated(TypeForCombobox(combobox), gfx::Rect(), true);
  ValidateGroup(*GroupForSection(section), VALIDATE_EDIT);
  SetEditabilityForSection(section);
}
