RegistrationManager::RegistrationManager(
    invalidation::InvalidationClient* invalidation_client)
    : invalidation_client_(invalidation_client) {
  DCHECK(invalidation_client_);
}
