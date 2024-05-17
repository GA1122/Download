double RegistrationManager::GetJitter() {
  return 2.0 * base::RandDouble() - 1.0;
}
