void AppCacheUpdateJob::LogHistogramStats(
      ResultType result, const GURL& failed_resource_url) {
  AppCacheHistograms::CountUpdateJobResult(result, manifest_url_.GetOrigin());
  if (result == UPDATE_OK)
    return;

  int percent_complete = 0;
  if (url_file_list_.size() > 0) {
    size_t actual_fetches_completed = url_fetches_completed_;
    if (!failed_resource_url.is_empty() && actual_fetches_completed)
      --actual_fetches_completed;
    percent_complete = (static_cast<double>(actual_fetches_completed) /
                            static_cast<double>(url_file_list_.size())) * 100.0;
    percent_complete = std::min(percent_complete, 99);
  }

  bool was_making_progress =
      base::Time::Now() - last_progress_time_ <
          base::TimeDelta::FromMinutes(5);

  bool off_origin_resource_failure =
      !failed_resource_url.is_empty() &&
          (failed_resource_url.GetOrigin() != manifest_url_.GetOrigin());

  AppCacheHistograms::LogUpdateFailureStats(
      manifest_url_.GetOrigin(),
      percent_complete,
      was_making_progress,
      off_origin_resource_failure);
}
