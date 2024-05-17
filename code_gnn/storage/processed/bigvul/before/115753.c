void JingleSessionManager::OnSessionDestroy(cricket::Session* cricket_session) {
  DCHECK(CalledOnValidThread());

  std::list<JingleSession*>::iterator it;
  for (it = sessions_.begin(); it != sessions_.end(); ++it) {
    if ((*it)->HasSession(cricket_session)) {
      (*it)->ReleaseSession();
      return;
    }
  }
}
