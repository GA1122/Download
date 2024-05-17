void RegistrationManager::FirePendingRegistrationsForTest() {
  DCHECK(CalledOnValidThread());
  for (RegistrationStatusMap::const_iterator it =
           registration_statuses_.begin();
       it != registration_statuses_.end(); ++it) {
    if (it->second->registration_timer.IsRunning()) {
      it->second->DoRegister();
    }
  }
}
