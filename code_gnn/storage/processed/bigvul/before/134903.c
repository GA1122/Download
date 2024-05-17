  void Initialize() {
    DBusThreadManager::Get()
        ->GetCryptohomeClient()
        ->WaitForServiceToBeAvailable(
            base::Bind(&SystemTokenCertDBInitializer::OnCryptohomeAvailable,
                       weak_ptr_factory_.GetWeakPtr()));
  }
