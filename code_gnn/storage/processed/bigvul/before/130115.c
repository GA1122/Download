  void MockStoreClientInfoBackup(const ClientInfo& client_info) {
    stored_client_info_backup_.reset(new ClientInfo);
    stored_client_info_backup_->client_id = client_info.client_id;
    stored_client_info_backup_->installation_date =
        client_info.installation_date;
    stored_client_info_backup_->reporting_enabled_date =
        client_info.reporting_enabled_date;

    if (client_info.client_id.empty())
      fake_client_info_backup_.reset();
  }
