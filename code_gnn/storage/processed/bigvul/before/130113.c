  scoped_ptr<ClientInfo> LoadFakeClientInfoBackup() {
    if (!fake_client_info_backup_)
      return scoped_ptr<ClientInfo>();

    scoped_ptr<ClientInfo> backup_copy(new ClientInfo);
    backup_copy->client_id = fake_client_info_backup_->client_id;
    backup_copy->installation_date =
        fake_client_info_backup_->installation_date;
    backup_copy->reporting_enabled_date =
        fake_client_info_backup_->reporting_enabled_date;
    return backup_copy.Pass();
  }
