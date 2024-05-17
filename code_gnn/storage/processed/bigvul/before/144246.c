void UserSelectionScreen::CheckUserStatus(const AccountId& account_id) {
  if (ScreenLocker::default_screen_locker())
    return;

  if (!token_handle_util_.get()) {
    token_handle_util_.reset(new TokenHandleUtil());
  }

  if (token_handle_util_->HasToken(account_id)) {
    token_handle_util_->CheckToken(
        account_id, base::Bind(&UserSelectionScreen::OnUserStatusChecked,
                               weak_factory_.GetWeakPtr()));
  }

  if (display_type_ == OobeUI::kLoginDisplay &&
      ShouldCheckNeedDircryptoMigration()) {
    if (!dircrypto_migration_checker_) {
      dircrypto_migration_checker_ =
          std::make_unique<DircryptoMigrationChecker>(this);
    }
    dircrypto_migration_checker_->Check(account_id);
  }
}
