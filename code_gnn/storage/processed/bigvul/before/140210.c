void WebResourceService::OnResourceRequestsAllowed() {
  int64_t delay = start_fetch_delay_ms_;
  if (prefs_->HasPrefPath(last_update_time_pref_name_)) {
    std::string last_update_pref =
        prefs_->GetString(last_update_time_pref_name_);
    if (!last_update_pref.empty()) {
      double last_update_value;
      base::StringToDouble(last_update_pref, &last_update_value);
      int64_t ms_until_update =
          cache_update_delay_ms_ -
          static_cast<int64_t>(
              (base::Time::Now() - base::Time::FromDoubleT(last_update_value))
                  .InMilliseconds());
      if (ms_until_update > start_fetch_delay_ms_)
        delay = ms_until_update;
    }
  }
  ScheduleFetch(delay);
}
