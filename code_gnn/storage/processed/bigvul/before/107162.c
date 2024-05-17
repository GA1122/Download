AutoFillManager::AutoFillManager(TabContents* tab_contents)
    : tab_contents_(tab_contents),
      personal_data_(NULL),
      download_manager_(tab_contents_->profile()),
      disable_download_manager_requests_(false),
      metric_logger_(new AutoFillMetrics),
      cc_infobar_(NULL) {
  DCHECK(tab_contents);

  personal_data_ =
      tab_contents_->profile()->GetOriginalProfile()->GetPersonalDataManager();
  download_manager_.SetObserver(this);
}