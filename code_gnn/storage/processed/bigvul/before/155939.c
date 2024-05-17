void PeopleHandler::InitializeSyncBlocker() {
  if (!web_ui())
    return;
  WebContents* web_contents = web_ui()->GetWebContents();
  if (web_contents) {
    ProfileSyncService* service = GetSyncService();
    const GURL current_url = web_contents->GetVisibleURL();
    if (service &&
        current_url == chrome::GetSettingsUrl(chrome::kSyncSetupSubPage)) {
      sync_blocker_ = service->GetSetupInProgressHandle();
    }
  }
}
