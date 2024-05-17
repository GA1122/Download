  void RunCryptohomeCheck(const AccountId& account_id, bool service_is_ready) {
    if (!service_is_ready) {
      LOG(ERROR) << "Cryptohome is not available.";
      return;
    }

    const cryptohome::Identification cryptohome_id(account_id);
    DBusThreadManager::Get()->GetCryptohomeClient()->NeedsDircryptoMigration(
        cryptohome_id,
        base::BindOnce(&DircryptoMigrationChecker::
                           OnCryptohomeNeedsDircryptoMigrationCallback,
                       weak_ptr_factory_.GetWeakPtr(), account_id));
  }
