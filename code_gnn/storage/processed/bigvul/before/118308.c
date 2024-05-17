void AutofillDialogViews::ShowDialogInMode(DialogMode dialog_mode) {
  loading_shield_->SetVisible(dialog_mode == LOADING);
  sign_in_web_view_->SetVisible(dialog_mode == SIGN_IN);
  notification_area_->SetVisible(dialog_mode == DETAIL_INPUT);
  scrollable_area_->SetVisible(dialog_mode == DETAIL_INPUT);
  FocusInitialView();
}
