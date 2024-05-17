void OfflinePageModelTaskified::MarkPageAccessed(int64_t offline_id) {
  auto task = base::MakeUnique<MarkPageAccessedTask>(store_.get(), offline_id,
                                                     GetCurrentTime());
  task_queue_.AddTask(std::move(task));
}
