void ProfileSyncService::OnClearServerDataSucceeded() {
  clear_server_data_timer_.Stop();

  if (clear_server_data_state_ != CLEAR_SUCCEEDED) {
    clear_server_data_state_ = CLEAR_SUCCEEDED;
    NotifyObservers();
  }
}
