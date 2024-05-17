void PeopleHandler::SyncStartupFailed() {
  engine_start_timer_.reset();

  CloseUI();
}
