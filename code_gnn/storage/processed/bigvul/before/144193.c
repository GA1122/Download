void LockContentsView::OnBigUserChanged() {
  const AccountId new_big_user =
      CurrentBigUserView()->GetCurrentUser()->basic_user_info->account_id;

  CurrentBigUserView()->RequestFocus();

  Shell::Get()->login_screen_controller()->OnFocusPod(new_big_user);
  UpdateEasyUnlockIconForUser(new_big_user);

  if (unlock_attempt_ > 0) {
    Shell::Get()->metrics()->login_metrics_recorder()->RecordNumLoginAttempts(
        unlock_attempt_, false  );

    unlock_attempt_ = 0;
  }

  OnDetachableBasePairingStatusChanged(
      detachable_base_model_->GetPairingStatus());
}
