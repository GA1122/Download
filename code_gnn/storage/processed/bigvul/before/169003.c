void OfflinePageModelTaskified::GetOfflineIdsForClientId(
    const ClientId& client_id,
    const MultipleOfflineIdCallback& callback) {
  auto task = GetPagesTask::CreateTaskMatchingClientIds(
      store_.get(), base::Bind(&WrapInMultipleItemsCallback, callback),
      {client_id});
  task_queue_.AddTask(std::move(task));
}
