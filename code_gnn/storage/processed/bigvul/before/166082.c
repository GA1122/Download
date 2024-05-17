void FileSystemManagerImpl::ReadDirectory(
    const GURL& path,
    blink::mojom::FileSystemOperationListenerPtr listener) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  FileSystemURL url(context_->CrackURL(path));
  base::Optional<base::File::Error> opt_error = ValidateFileSystemURL(url);
  if (opt_error) {
    listener->ErrorOccurred(opt_error.value());
    return;
  }
  if (!security_policy_->CanReadFileSystemFile(process_id_, url)) {
    listener->ErrorOccurred(base::File::FILE_ERROR_SECURITY);
    return;
  }

  OperationListenerID listener_id = AddOpListener(std::move(listener));
  operation_runner()->ReadDirectory(
      url, base::BindRepeating(&FileSystemManagerImpl::DidReadDirectory,
                               GetWeakPtr(), listener_id));
}
