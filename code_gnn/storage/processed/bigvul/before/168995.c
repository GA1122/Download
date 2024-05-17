void OfflinePageModelTaskified::ClearLegacyTemporaryPages() {
  auto task = base::MakeUnique<ClearLegacyTemporaryPagesTask>(
      store_.get(), policy_controller_.get(),
      archive_manager_->GetPersistentArchivesDir());
  task_queue_.AddTask(std::move(task));
}
