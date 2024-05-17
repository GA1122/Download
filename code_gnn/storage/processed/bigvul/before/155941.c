void PeopleHandler::MarkFirstSetupComplete() {
  signin::SetUserSkippedPromo(profile_);

  ProfileSyncService* service = GetSyncService();
  if (!service || service->IsFirstSetupComplete())
    return;

  base::FilePath profile_file_path = profile_->GetPath();
  ProfileMetrics::LogProfileSyncSignIn(profile_file_path);

  sync_blocker_.reset();
  service->SetFirstSetupComplete();
  FireWebUIListener("sync-settings-saved");
}
