SessionService* SessionModelAssociator::GetSessionService() {
  DCHECK(CalledOnValidThread());
  DCHECK(sync_service_);
  Profile* profile = sync_service_->profile();
  DCHECK(profile);
  SessionService* sessions_service =
      SessionServiceFactory::GetForProfile(profile);
  DCHECK(sessions_service);
  return sessions_service;
}
