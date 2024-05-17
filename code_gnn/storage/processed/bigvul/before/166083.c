void FileSystemManagerImpl::ReadDirectorySync(
    const GURL& path,
    ReadDirectorySyncCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  FileSystemURL url(context_->CrackURL(path));
  base::Optional<base::File::Error> opt_error = ValidateFileSystemURL(url);
  if (opt_error) {
    std::move(callback).Run(std::vector<filesystem::mojom::DirectoryEntryPtr>(),
                            opt_error.value());
    return;
  }
  if (!security_policy_->CanReadFileSystemFile(process_id_, url)) {
    std::move(callback).Run(std::vector<filesystem::mojom::DirectoryEntryPtr>(),
                            base::File::FILE_ERROR_SECURITY);
    return;
  }

  operation_runner()->ReadDirectory(
      url, base::BindRepeating(
               &FileSystemManagerImpl::DidReadDirectorySync, GetWeakPtr(),
               base::Owned(
                   new ReadDirectorySyncCallbackEntry(std::move(callback)))));
}
