AutoFillManager::AutoFillManager()
    : tab_contents_(NULL),
      personal_data_(NULL),
      download_manager_(NULL),
      disable_download_manager_requests_(true),
      metric_logger_(new AutoFillMetrics),
      cc_infobar_(NULL) {
}
