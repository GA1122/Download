void JingleSessionManager::SessionDestroyed(JingleSession* jingle_session) {
  std::list<JingleSession*>::iterator it =
      std::find(sessions_.begin(), sessions_.end(), jingle_session);
  CHECK(it != sessions_.end());
  cricket::Session* cricket_session = jingle_session->ReleaseSession();
  cricket_session_manager_->DestroySession(cricket_session);
  sessions_.erase(it);
}
