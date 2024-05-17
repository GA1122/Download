void ProfileSyncService::ClearServerData() {
  clear_server_data_state_ = CLEAR_CLEARING;
  clear_server_data_timer_.Start(FROM_HERE,
      base::TimeDelta::FromSeconds(kSyncClearDataTimeoutInSeconds), this,
      &ProfileSyncService::OnClearServerDataTimeout);
  backend_->RequestClearServerData();
}
