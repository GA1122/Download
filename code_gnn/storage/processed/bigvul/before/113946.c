void RegistrationManager::MarkAllRegistrationsLost() {
  DCHECK(CalledOnValidThread());
  for (RegistrationStatusMap::const_iterator it =
           registration_statuses_.begin();
       it != registration_statuses_.end(); ++it) {
    if (IsIdRegistered(it->first)) {
      MarkRegistrationLost(it->first);
    }
  }
}
