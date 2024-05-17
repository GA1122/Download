  void Check(const AccountId& account_id) {
    focused_user_ = account_id;

    if (!policy::BrowserPolicyConnector::IsNonEnterpriseUser(
            account_id.GetUserEmail())) {
      UpdateUI(account_id, false);
      return;
    }

    auto it = needs_dircrypto_migration_cache_.find(account_id);
    if (it != needs_dircrypto_migration_cache_.end()) {
      UpdateUI(account_id, it->second);
      return;
    }

    if (!IsUserAllowedForARC(account_id)) {
      UpdateUI(account_id, false);
      return;
    }

    DBusThreadManager::Get()
        ->GetCryptohomeClient()
        ->WaitForServiceToBeAvailable(
            base::Bind(&DircryptoMigrationChecker::RunCryptohomeCheck,
                       weak_ptr_factory_.GetWeakPtr(), account_id));
  }
