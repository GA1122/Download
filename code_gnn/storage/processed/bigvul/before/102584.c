bool EnterpriseEnrollmentScreen::GetInitialUser(std::string* user) {
  chromeos::CryptohomeLibrary* cryptohome =
      chromeos::CrosLibrary::Get()->GetCryptohomeLibrary();
  if (cryptohome &&
      cryptohome->InstallAttributesIsReady() &&
      !cryptohome->InstallAttributesIsFirstInstall()) {
    std::string value;
    if (cryptohome->InstallAttributesGet("enterprise.owned", &value) &&
        value == "true") {
      if (cryptohome->InstallAttributesGet("enterprise.user", &value)) {
        *user = value;
        actor_->SetEditableUser(false);
        return true;
      }
    }
    LOG(ERROR) << "Enrollment will not finish because the InstallAttrs has "
               << "been locked already but does not contain valid data.";
  }
  return false;
}
