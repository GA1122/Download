std::unique_ptr<base::ListValue> PeopleHandler::GetStoredAccountsList() {
  std::unique_ptr<base::ListValue> accounts_list =
      std::make_unique<base::ListValue>();
  bool dice_enabled =
      AccountConsistencyModeManager::IsDiceEnabledForProfile(profile_);

  if (!dice_enabled && !unified_consent::IsUnifiedConsentFeatureEnabled())
    return accounts_list;

  AccountTrackerService* account_tracker =
      AccountTrackerServiceFactory::GetForProfile(profile_);

  if (dice_enabled) {
    std::vector<AccountInfo> accounts =
        signin_ui_util::GetAccountsForDicePromos(profile_);
    accounts_list->Reserve(accounts.size());
    for (auto const& account : accounts) {
      accounts_list->GetList().push_back(
          GetAccountValue(account, account_tracker));
    }
  } else {
    std::string primary_account = SigninManagerFactory::GetForProfile(profile_)
                                      ->GetAuthenticatedAccountId();
    if (!primary_account.empty()) {
      accounts_list->GetList().push_back(GetAccountValue(
          account_tracker->GetAccountInfo(primary_account), account_tracker));
    }
  }

  return accounts_list;
}
