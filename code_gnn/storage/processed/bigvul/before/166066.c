void FileSystemManagerImpl::DidReadDirectorySync(
    ReadDirectorySyncCallbackEntry* callback_entry,
    base::File::Error result,
    std::vector<filesystem::mojom::DirectoryEntry> entries,
    bool has_more) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  for (const auto& entry : entries) {
    callback_entry->entries.emplace_back(
        filesystem::mojom::DirectoryEntry::New(std::move(entry)));
  }
  if (result != base::File::FILE_OK || !has_more) {
    std::move(callback_entry->callback)
        .Run(std::move(callback_entry->entries), result);
  }
}
