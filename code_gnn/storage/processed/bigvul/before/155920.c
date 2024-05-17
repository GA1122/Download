void PeopleHandler::DisplayGaiaLogin(signin_metrics::AccessPoint access_point) {
  DCHECK(!sync_startup_tracker_);
  configuring_sync_ = false;
  DisplayGaiaLoginInNewTabOrWindow(access_point);
}
