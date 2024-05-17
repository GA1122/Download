AutoFillManager::AutoFillManager(TabContents* tab_contents,
                                 PersonalDataManager* personal_data)
    : tab_contents_(tab_contents),
      personal_data_(personal_data),
      download_manager_(NULL),
      disable_download_manager_requests_(true),
      metric_logger_(new AutoFillMetrics),
      cc_infobar_(NULL) {
  DCHECK(tab_contents);
}
