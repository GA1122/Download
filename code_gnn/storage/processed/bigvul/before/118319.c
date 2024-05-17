void AutofillDialogViews::UpdateButtonStrip() {
  button_strip_extra_view_->SetVisible(
      GetDialogButtons() != ui::DIALOG_BUTTON_NONE);
  UpdateButtonStripExtraView();
  GetDialogClientView()->UpdateDialogButtons();

  ContentsPreferredSizeChanged();
}
