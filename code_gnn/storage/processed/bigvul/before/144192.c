void LockContentsView::OnAuthenticate(bool auth_success) {
  if (auth_success) {
    auth_error_bubble_->Close();
    detachable_base_error_bubble_->Close();

    if (CurrentBigUserView()->auth_user() &&
        detachable_base_model_->GetPairingStatus() ==
            DetachableBasePairingStatus::kAuthenticated) {
      detachable_base_model_->SetPairedBaseAsLastUsedByUser(
          *CurrentBigUserView()->GetCurrentUser()->basic_user_info);
    }
  } else {
    ++unlock_attempt_;
    ShowAuthErrorMessage();
  }
}
