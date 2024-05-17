bool RegistrationManager::IsIdRegistered(
    const invalidation::ObjectId& id) const {
  DCHECK(CalledOnValidThread());
  RegistrationStatusMap::const_iterator it =
      registration_statuses_.find(id);
  return it != registration_statuses_.end() &&
      it->second->state == invalidation::InvalidationListener::REGISTERED;
}
