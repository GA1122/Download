  void UpdateUI(const AccountId& account_id, bool needs_migration) {
    if (account_id != focused_user_)
      return;

    owner_->ShowBannerMessage(
        needs_migration ? l10n_util::GetStringUTF16(
                              IDS_LOGIN_NEEDS_DIRCRYPTO_MIGRATION_BANNER)
                        : base::string16(),
        needs_migration);
  }
