base::Value BuildRealtimeReport(Profile* profile, base::Value event) {
  base::Value context(base::Value::Type::DICTIONARY);

  ProfileAttributesStorage& storage =
      g_browser_process->profile_manager()->GetProfileAttributesStorage();
  ProfileAttributesEntry* entry = nullptr;
  if (storage.GetProfileAttributesWithPath(profile->GetPath(), &entry)) {
    context.SetStringPath("profile.profileName", entry->GetName());
    context.SetStringPath("profile.gaiaEmail", entry->GetUserName());
  }

  context.SetStringPath("profile.profilePath", profile->GetPath().value());
  context.SetStringPath("browser.userAgent", GetUserAgent());

  base::Value report(base::Value::Type::DICTIONARY);
  report.SetKey(policy::RealtimeReportingJobConfiguration::kContextKey,
                std::move(context));
  report.SetKey(policy::RealtimeReportingJobConfiguration::kEventKey,
                std::move(event));
  return report;
}
