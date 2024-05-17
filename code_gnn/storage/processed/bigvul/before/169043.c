void OfflinePageModelImpl::DeletePagesMissingArchiveFile(
    const std::set<base::FilePath>& archive_paths) {
  std::vector<int64_t> ids_of_pages_missing_archive_file;
  for (const auto& id_page_pair : offline_pages_) {
    if (archive_paths.count(id_page_pair.second.file_path) == 0UL)
      ids_of_pages_missing_archive_file.push_back(id_page_pair.first);
  }

  if (ids_of_pages_missing_archive_file.empty())
    return;

  DeletePagesByOfflineId(
      ids_of_pages_missing_archive_file,
      base::Bind(&OfflinePageModelImpl::OnDeletePagesMissingArchiveFileDone,
                 weak_ptr_factory_.GetWeakPtr(),
                 ids_of_pages_missing_archive_file));
}
