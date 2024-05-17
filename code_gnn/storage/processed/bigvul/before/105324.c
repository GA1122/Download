AutofillManager::AutofillManager(TabContentsWrapper* tab_contents)
    : TabContentsObserver(tab_contents->tab_contents()),
      tab_contents_wrapper_(tab_contents),
      personal_data_(NULL),
      download_manager_(tab_contents->profile()),
      disable_download_manager_requests_(false),
      metric_logger_(new AutofillMetrics),
      has_logged_autofill_enabled_(false),
      has_logged_address_suggestions_count_(false) {
  DCHECK(tab_contents);

  personal_data_ =
      tab_contents->profile()->GetOriginalProfile()->GetPersonalDataManager();
  download_manager_.SetObserver(this);
}
