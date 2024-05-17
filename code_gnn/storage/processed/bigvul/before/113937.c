void RegistrationManager::DisableId(const invalidation::ObjectId& id) {
  DCHECK(CalledOnValidThread());
  RegistrationStatusMap::const_iterator it = registration_statuses_.find(id);
  if (it == registration_statuses_.end()) {
    DLOG(WARNING) << "Attempt to disable non-existent registration for "
                  << ObjectIdToString(id);
    return;
  }
  it->second->Disable();
}
