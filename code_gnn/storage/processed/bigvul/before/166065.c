void FileSystemManagerImpl::DidReadDirectory(
    OperationListenerID listener_id,
    base::File::Error result,
    std::vector<filesystem::mojom::DirectoryEntry> entries,
    bool has_more) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  blink::mojom::FileSystemOperationListener* listener =
      GetOpListener(listener_id);
  if (!listener)
    return;
  if (result != base::File::FILE_OK) {
    DCHECK(!has_more);
    listener->ErrorOccurred(result);
    RemoveOpListener(listener_id);
    return;
  }
  std::vector<filesystem::mojom::DirectoryEntryPtr> entry_struct_ptrs;
  for (const auto& entry : entries) {
    entry_struct_ptrs.emplace_back(
        filesystem::mojom::DirectoryEntry::New(entry));
  }
  listener->ResultsRetrieved(std::move(entry_struct_ptrs), has_more);
  if (!has_more)
    RemoveOpListener(listener_id);
}
