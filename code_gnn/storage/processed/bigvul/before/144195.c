void LockContentsView::OnDetachableBasePairingStatusChanged(
    DetachableBasePairingStatus pairing_status) {
  if (!CurrentBigUserView() || !CurrentBigUserView()->auth_user() ||
      pairing_status == DetachableBasePairingStatus::kNone ||
      (pairing_status == DetachableBasePairingStatus::kAuthenticated &&
       detachable_base_model_->PairedBaseMatchesLastUsedByUser(
           *CurrentBigUserView()->GetCurrentUser()->basic_user_info))) {
    detachable_base_error_bubble_->Close();
    return;
  }

  auth_error_bubble_->Close();

  base::string16 error_text =
      l10n_util::GetStringUTF16(IDS_ASH_LOGIN_ERROR_DETACHABLE_BASE_CHANGED);

  views::Label* label =
      new views::Label(error_text, views::style::CONTEXT_MESSAGE_BOX_BODY_TEXT,
                       views::style::STYLE_PRIMARY);
  label->SetMultiLine(true);
  label->SetAutoColorReadabilityEnabled(false);
  label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  label->SetEnabledColor(SK_ColorWHITE);

  detachable_base_error_bubble_->ShowErrorBubble(
      label, CurrentBigUserView()->auth_user()->password_view()  ,
      LoginBubble::kFlagPersistent);

  if (GetWidget()->IsActive())
    GetWidget()->GetFocusManager()->ClearFocus();
}
