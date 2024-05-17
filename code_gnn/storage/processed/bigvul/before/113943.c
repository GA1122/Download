ObjectIdSet RegistrationManager::GetRegisteredIds() const {
  DCHECK(CalledOnValidThread());
  ObjectIdSet ids;
  for (RegistrationStatusMap::const_iterator it =
           registration_statuses_.begin();
       it != registration_statuses_.end(); ++it) {
    if (IsIdRegistered(it->first)) {
      ids.insert(it->first);
    }
  }
  return ids;
}
