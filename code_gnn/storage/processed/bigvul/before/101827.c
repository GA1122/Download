void Browser::ShowSyncSetup() {
  ProfileSyncService* service =
      profile()->GetOriginalProfile()->GetProfileSyncService();
  if (service->HasSyncSetupCompleted())
    ShowOptionsTab(chrome::kSyncSetupSubPage);
  else
    service->ShowLoginDialog();
}
