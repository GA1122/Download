void AutofillDialogViews::ValidateSection(DialogSection section) {
  ValidateGroup(*GroupForSection(section), VALIDATE_EDIT);
}
