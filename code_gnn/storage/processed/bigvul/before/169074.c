void OfflinePageModelImpl::OnAddPageDone(const OfflinePageItem& offline_page,
                                         const AddPageCallback& callback,
                                         ItemActionStatus status) {
  AddPageResult result;
  if (status == ItemActionStatus::SUCCESS) {
    offline_pages_[offline_page.offline_id] = offline_page;
    result = AddPageResult::SUCCESS;
    ReportPageHistogramAfterSave(policy_controller_.get(), offline_pages_,
                                 offline_page, GetCurrentTime());
    offline_event_logger_.RecordPageSaved(offline_page.client_id.name_space,
                                          offline_page.url.spec(),
                                          offline_page.offline_id);
  } else if (status == ItemActionStatus::ALREADY_EXISTS) {
    archive_manager_->DeleteArchive(offline_page.file_path,
                                    base::Bind([](bool) {}));
    result = AddPageResult::ALREADY_EXISTS;
  } else {
    result = AddPageResult::STORE_FAILURE;
  }

  callback.Run(result, offline_page.offline_id);

  if (status == ItemActionStatus::SUCCESS) {
    for (Observer& observer : observers_)
      observer.OfflinePageAdded(this, offline_page);
  }
}
