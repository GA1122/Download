int ExtensionInstallDialogView::GetDialogButtons() const {
  int buttons = prompt_->GetDialogButtons();
  DCHECK_GT(buttons & ui::DIALOG_BUTTON_CANCEL, 0);
  return buttons;
}
