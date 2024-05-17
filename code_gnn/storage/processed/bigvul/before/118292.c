bool AutofillDialogViews::SaveDetailsLocally() {
  DCHECK(save_in_chrome_checkbox_->visible());
  return save_in_chrome_checkbox_->checked();
}
