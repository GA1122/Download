void FileSystemManagerImpl::Open(const url::Origin& origin,
                                 blink::mojom::FileSystemType file_system_type,
                                 OpenCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (file_system_type == blink::mojom::FileSystemType::kTemporary) {
    RecordAction(base::UserMetricsAction("OpenFileSystemTemporary"));
  } else if (file_system_type == blink::mojom::FileSystemType::kPersistent) {
    RecordAction(base::UserMetricsAction("OpenFileSystemPersistent"));
  }
  context_->OpenFileSystem(
      origin.GetURL(),
      mojo::ConvertTo<storage::FileSystemType>(file_system_type),
      storage::OPEN_FILE_SYSTEM_CREATE_IF_NONEXISTENT,
      base::BindOnce(&FileSystemManagerImpl::DidOpenFileSystem, GetWeakPtr(),
                     std::move(callback)));
}
