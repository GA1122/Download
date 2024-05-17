void FileSystemManagerImpl::Exists(const GURL& path,
                                   bool is_directory,
                                   ExistsCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  FileSystemURL url(context_->CrackURL(path));
  base::Optional<base::File::Error> opt_error = ValidateFileSystemURL(url);
  if (opt_error) {
    std::move(callback).Run(opt_error.value());
    return;
  }
  if (!security_policy_->CanReadFileSystemFile(process_id_, url)) {
    std::move(callback).Run(base::File::FILE_ERROR_SECURITY);
    return;
  }

  if (is_directory) {
    operation_runner()->DirectoryExists(
        url, base::BindRepeating(&FileSystemManagerImpl::DidFinish,
                                 GetWeakPtr(), base::Passed(&callback)));
  } else {
    operation_runner()->FileExists(
        url, base::BindRepeating(&FileSystemManagerImpl::DidFinish,
                                 GetWeakPtr(), base::Passed(&callback)));
  }
}
