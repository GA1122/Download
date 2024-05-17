  void SetFilterActions(FileMetricsProvider::Params* params,
                        const FileMetricsProvider::FilterAction* actions,
                        size_t count) {
    filter_actions_ = actions;
    filter_actions_remaining_ = count;
    params->filter = base::Bind(&FileMetricsProviderTest::FilterSourcePath,
                                base::Unretained(this));
  }
