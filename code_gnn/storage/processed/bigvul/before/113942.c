    RegistrationManager::GetPendingRegistrationsForTest() const {
  DCHECK(CalledOnValidThread());
  PendingRegistrationMap pending_registrations;
  for (RegistrationStatusMap::const_iterator it =
           registration_statuses_.begin();
       it != registration_statuses_.end(); ++it) {
    const invalidation::ObjectId& id = it->first;
    RegistrationStatus* status = it->second;
    if (status->registration_timer.IsRunning()) {
      pending_registrations[id].last_registration_request =
          status->last_registration_request;
      pending_registrations[id].registration_attempt =
          status->last_registration_attempt;
      pending_registrations[id].delay = status->delay;
      pending_registrations[id].actual_delay =
          status->registration_timer.GetCurrentDelay();
    }
  }
  return pending_registrations;
}
