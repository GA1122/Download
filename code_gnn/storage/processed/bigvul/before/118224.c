int AutofillDialogViews::GetDefaultDialogButton() const {
  if (GetDialogButtons() & ui::DIALOG_BUTTON_OK)
    return ui::DIALOG_BUTTON_OK;

  return ui::DIALOG_BUTTON_NONE;
}
