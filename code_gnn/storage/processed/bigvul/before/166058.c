void FileSystemManagerImpl::CreateSnapshotFile(
    const GURL& file_path,
    CreateSnapshotFileCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  FileSystemURL url(context_->CrackURL(file_path));

  base::Optional<base::File::Error> opt_error = ValidateFileSystemURL(url);
  if (opt_error) {
    std::move(callback).Run(base::File::Info(), base::FilePath(),
                            opt_error.value(), nullptr);
    return;
  }
  if (!security_policy_->CanReadFileSystemFile(process_id_, url)) {
    std::move(callback).Run(base::File::Info(), base::FilePath(),
                            base::File::FILE_ERROR_SECURITY, nullptr);
    return;
  }

  FileSystemBackend* backend = context_->GetFileSystemBackend(url.type());
  if (backend->SupportsStreaming(url)) {
    operation_runner()->GetMetadata(
        url,
        FileSystemOperation::GET_METADATA_FIELD_IS_DIRECTORY |
            FileSystemOperation::GET_METADATA_FIELD_SIZE |
            FileSystemOperation::GET_METADATA_FIELD_LAST_MODIFIED,
        base::BindRepeating(&FileSystemManagerImpl::DidGetMetadataForStreaming,
                            GetWeakPtr(), base::Passed(&callback)));
  } else {
    operation_runner()->CreateSnapshotFile(
        url, base::BindRepeating(&FileSystemManagerImpl::DidCreateSnapshot,
                                 GetWeakPtr(), base::Passed(&callback), url));
  }
}
