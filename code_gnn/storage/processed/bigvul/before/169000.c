void OfflinePageModelTaskified::GetAllPages(
    const MultipleOfflinePageItemCallback& callback) {
  auto task = GetPagesTask::CreateTaskMatchingAllPages(store_.get(), callback);
  task_queue_.AddTask(std::move(task));
}
