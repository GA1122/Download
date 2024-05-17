void OfflinePageModelTaskified::CheckTemporaryPagesConsistency() {
  auto task = base::MakeUnique<TemporaryPagesConsistencyCheckTask>(
      store_.get(), policy_controller_.get(),
      archive_manager_->GetTemporaryArchivesDir());
  task_queue_.AddTask(std::move(task));
}
