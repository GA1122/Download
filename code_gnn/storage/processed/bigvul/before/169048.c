void OfflinePageModelImpl::FinalizeModelLoad() {
  is_loaded_ = true;


  UMA_HISTOGRAM_BOOLEAN("OfflinePages.Model.FinalLoadSuccessful",
                        store_->state() == StoreState::LOADED);

  for (Observer& observer : observers_)
    observer.OfflinePageModelLoaded(this);

  for (const auto& delayed_task : delayed_tasks_)
    delayed_task.Run();
  delayed_tasks_.clear();

  PostClearStorageIfNeededTask(true  );
}
