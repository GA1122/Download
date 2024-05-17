void FileSystemManagerImpl::WriteSync(const GURL& file_path,
                                      const std::string& blob_uuid,
                                      int64_t position,
                                      WriteSyncCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  FileSystemURL url(context_->CrackURL(file_path));
  base::Optional<base::File::Error> opt_error = ValidateFileSystemURL(url);
  if (opt_error) {
    std::move(callback).Run(0, opt_error.value());
    return;
  }
  if (!security_policy_->CanWriteFileSystemFile(process_id_, url)) {
    std::move(callback).Run(0, base::File::FILE_ERROR_SECURITY);
    return;
  }
  std::unique_ptr<storage::BlobDataHandle> blob =
      blob_storage_context_->context()->GetBlobDataFromUUID(blob_uuid);

  operation_runner()->Write(
      url, std::move(blob), position,
      base::BindRepeating(
          &FileSystemManagerImpl::DidWriteSync, GetWeakPtr(),
          base::Owned(new WriteSyncCallbackEntry(std::move(callback)))));
}
