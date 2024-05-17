void OfflinePageModelTaskified::CheckPersistentPagesConsistency() {
  auto task = base::MakeUnique<PersistentPagesConsistencyCheckTask>(
      store_.get(), policy_controller_.get(),
      archive_manager_->GetPersistentArchivesDir());
  task_queue_.AddTask(std::move(task));
}
