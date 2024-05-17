EnterpriseEnrollmentScreen::EnterpriseEnrollmentScreen(
    ScreenObserver* observer,
    EnterpriseEnrollmentScreenActor* actor)
    : WizardScreen(observer),
      actor_(actor),
      is_showing_(false),
      ALLOW_THIS_IN_INITIALIZER_LIST(runnable_method_factory_(this)) {
  actor_->SetController(this);
  chromeos::CryptohomeLibrary* cryptohome =
      chromeos::CrosLibrary::Get()->GetCryptohomeLibrary();
  if (cryptohome) {
    if (cryptohome->TpmIsEnabled() &&
        !cryptohome->TpmIsBeingOwned() &&
        !cryptohome->TpmIsOwned()) {
      cryptohome->TpmCanAttemptOwnership();
    }
  }
}
