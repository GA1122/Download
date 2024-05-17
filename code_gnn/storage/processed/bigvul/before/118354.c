void CardUnmaskPromptViews::DisableAndWaitForVerification() {
  SetInputsEnabled(false);
  progress_overlay_->SetOpacity(0.0);
  progress_overlay_->SetVisible(true);
  progress_throbber_->Start();
  overlay_animation_.Show();
  GetDialogClientView()->UpdateDialogButtons();
  Layout();
}
