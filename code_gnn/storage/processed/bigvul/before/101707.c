void Browser::NewWindowWithProfile(Profile* profile) {
  UserMetrics::RecordAction(UserMetricsAction("NewWindow"));
  SessionService* session_service =
      SessionServiceFactory::GetForProfile(profile->GetOriginalProfile());
  if (!session_service ||
      !session_service->RestoreIfNecessary(std::vector<GURL>())) {
    Browser::OpenEmptyWindow(profile->GetOriginalProfile());
  }
}
