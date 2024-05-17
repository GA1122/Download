RegistrationManager::RegistrationStatus::RegistrationStatus(
    const invalidation::ObjectId& id, RegistrationManager* manager)
    : id(id),
      registration_manager(manager),
      enabled(true),
      state(invalidation::InvalidationListener::UNREGISTERED) {
  DCHECK(registration_manager);
}
