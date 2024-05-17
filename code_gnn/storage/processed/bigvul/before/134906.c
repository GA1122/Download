  void OnCryptohomeAvailable(bool available) {
    if (!available) {
      LOG(ERROR) << "SystemTokenCertDBInitializer: Failed to wait for "
                    "cryptohome to become available.";
      return;
    }

    VLOG(1) << "SystemTokenCertDBInitializer: Cryptohome available.";
    DBusThreadManager::Get()->GetCryptohomeClient()->TpmIsReady(
        base::Bind(&SystemTokenCertDBInitializer::OnGotTpmIsReady,
                   weak_ptr_factory_.GetWeakPtr()));
  }
