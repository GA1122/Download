void FileSystemManagerImpl::Truncate(
    const GURL& file_path,
    int64_t length,
    blink::mojom::FileSystemCancellableOperationRequest op_request,
    TruncateCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  FileSystemURL url(context_->CrackURL(file_path));
  base::Optional<base::File::Error> opt_error = ValidateFileSystemURL(url);
  if (opt_error) {
    std::move(callback).Run(opt_error.value());
    return;
  }
  if (!security_policy_->CanWriteFileSystemFile(process_id_, url)) {
    std::move(callback).Run(base::File::FILE_ERROR_SECURITY);
    return;
  }

  OperationID op_id = operation_runner()->Truncate(
      url, length,
      base::BindRepeating(&FileSystemManagerImpl::DidFinish, GetWeakPtr(),
                          base::Passed(&callback)));
  cancellable_operations_.AddBinding(
      std::make_unique<FileSystemCancellableOperationImpl>(op_id, this),
      std::move(op_request));
}
