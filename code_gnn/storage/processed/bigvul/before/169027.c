void OfflinePageModelTaskified::SavePage(
    const SavePageParams& save_page_params,
    std::unique_ptr<OfflinePageArchiver> archiver,
    const SavePageCallback& callback) {
  auto task = base::MakeUnique<CreateArchiveTask>(
      GetArchiveDirectory(save_page_params.client_id.name_space),
      save_page_params, archiver.get(),
      base::Bind(&OfflinePageModelTaskified::OnCreateArchiveDone,
                 weak_ptr_factory_.GetWeakPtr(), callback));
  pending_archivers_.push_back(std::move(archiver));
  task_queue_.AddTask(std::move(task));
}
