void PeopleHandler::DisplayTimeout() {
  engine_start_timer_.reset();

  sync_startup_tracker_.reset();

  FireWebUIListener("page-status-changed", base::Value(kTimeoutPageStatus));
}
