  void AccountReady(const AccountInfo& info,
                    const identity::AccountState& state) {
    GetIdentityManager().GetAccessToken(
        host_->delegate_->GetAccountId().GetUserEmail(), {},
        kIdentityConsumerId,
        base::BindOnce(&AccountTokenDelegate::GotChromeAccessToken,
                       base::Unretained(this)));
  }
