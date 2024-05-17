  void OnGotTpmIsReady(base::Optional<bool> tpm_is_ready) {
    if (!tpm_is_ready.has_value() || !tpm_is_ready.value()) {
      VLOG(1) << "SystemTokenCertDBInitializer: TPM is not ready - not loading "
                 "system token.";
      if (ShallAttemptTpmOwnership()) {
        LOG(WARNING) << "Request attempting TPM ownership.";
        DBusThreadManager::Get()->GetCryptohomeClient()->TpmCanAttemptOwnership(
            EmptyVoidDBusMethodCallback());
      }

      return;
    }
    VLOG(1)
        << "SystemTokenCertDBInitializer: TPM is ready, loading system token.";
    TPMTokenLoader::Get()->EnsureStarted();
    base::Callback<void(crypto::ScopedPK11Slot)> callback =
        base::BindRepeating(&SystemTokenCertDBInitializer::InitializeDatabase,
                            weak_ptr_factory_.GetWeakPtr());
    content::BrowserThread::PostTask(
        content::BrowserThread::IO, FROM_HERE,
        base::BindOnce(&GetSystemSlotOnIOThread, callback));
  }
