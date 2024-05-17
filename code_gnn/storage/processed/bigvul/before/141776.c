std::unique_ptr<metrics::FileMetricsProvider> CreateFileMetricsProvider(
    bool metrics_reporting_enabled) {
  std::unique_ptr<metrics::FileMetricsProvider> file_metrics_provider(
      new metrics::FileMetricsProvider(g_browser_process->local_state()));

  base::FilePath user_data_dir;
  if (base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir)) {
    RegisterOrRemovePreviousRunMetricsFile(
        metrics_reporting_enabled, user_data_dir,
        kCrashpadHistogramAllocatorName,
        metrics::FileMetricsProvider::
            ASSOCIATE_INTERNAL_PROFILE_OR_PREVIOUS_RUN,
        file_metrics_provider.get());

    base::FilePath browser_metrics_upload_dir =
        user_data_dir.AppendASCII(kBrowserMetricsName);
    if (metrics_reporting_enabled) {
      metrics::FileMetricsProvider::Params browser_metrics_params(
          browser_metrics_upload_dir,
          metrics::FileMetricsProvider::SOURCE_HISTOGRAMS_ATOMIC_DIR,
          metrics::FileMetricsProvider::ASSOCIATE_INTERNAL_PROFILE,
          kBrowserMetricsName);
      browser_metrics_params.max_dir_kib = kMaxHistogramStorageKiB;
      browser_metrics_params.filter = base::BindRepeating(
          &ChromeMetricsServiceClient::FilterBrowserMetricsFiles);
      file_metrics_provider->RegisterSource(browser_metrics_params);

      base::FilePath active_path;
      base::GlobalHistogramAllocator::ConstructFilePaths(
          user_data_dir, kCrashpadHistogramAllocatorName, nullptr, &active_path,
          nullptr);
      file_metrics_provider->RegisterSource(
          metrics::FileMetricsProvider::Params(
              active_path,
              metrics::FileMetricsProvider::SOURCE_HISTOGRAMS_ACTIVE_FILE,
              metrics::FileMetricsProvider::ASSOCIATE_CURRENT_RUN));
    } else {
      base::PostTaskWithTraits(
          FROM_HERE,
          {base::MayBlock(), base::TaskPriority::BEST_EFFORT,
           base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN},
          base::BindOnce(base::IgnoreResult(&base::DeleteFile),
                         std::move(browser_metrics_upload_dir),
                          true));
    }
  }

#if defined(OS_WIN)
  base::FilePath program_dir;
  base::PathService::Get(base::DIR_EXE, &program_dir);
  file_metrics_provider->RegisterSource(metrics::FileMetricsProvider::Params(
      program_dir.AppendASCII(installer::kSetupHistogramAllocatorName),
      metrics::FileMetricsProvider::SOURCE_HISTOGRAMS_ATOMIC_DIR,
      metrics::FileMetricsProvider::ASSOCIATE_CURRENT_RUN,
      installer::kSetupHistogramAllocatorName));

  if (!user_data_dir.empty()) {
    base::FilePath notification_helper_metrics_upload_dir =
        user_data_dir.AppendASCII(
            notification_helper::kNotificationHelperHistogramAllocatorName);

    if (metrics_reporting_enabled) {
      file_metrics_provider->RegisterSource(
          metrics::FileMetricsProvider::Params(
              notification_helper_metrics_upload_dir,
              metrics::FileMetricsProvider::SOURCE_HISTOGRAMS_ATOMIC_DIR,
              metrics::FileMetricsProvider::ASSOCIATE_CURRENT_RUN,
              notification_helper::kNotificationHelperHistogramAllocatorName));
    } else {
      base::PostTaskWithTraits(
          FROM_HERE,
          {base::MayBlock(), base::TaskPriority::BEST_EFFORT,
           base::TaskShutdownBehavior::CONTINUE_ON_SHUTDOWN},
          base::BindOnce(base::IgnoreResult(&base::DeleteFile),
                         std::move(notification_helper_metrics_upload_dir),
                          true));
    }
  }
#endif

  return file_metrics_provider;
}
