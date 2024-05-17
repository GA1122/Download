void SafeBrowsingPrivateEventRouter::ReportRealtimeEvent(const char* name,
                                                         base::Value event) {
  base::Time::Exploded now_exploded;
  base::Time::Now().UTCExplode(&now_exploded);
  std::string now_str = base::StringPrintf(
      "%d-%02d-%02dT%02d:%02d:%02d.%03dZ", now_exploded.year,
      now_exploded.month, now_exploded.day_of_month, now_exploded.hour,
      now_exploded.minute, now_exploded.second, now_exploded.millisecond);

  base::Value wrapper(base::Value::Type::DICTIONARY);
  wrapper.SetStringKey("time", now_str);
  wrapper.SetKey(name, std::move(event));

  client_->UploadRealtimeReport(
      BuildRealtimeReport(Profile::FromBrowserContext(context_),
                          std::move(wrapper)),
      base::DoNothing());
}
