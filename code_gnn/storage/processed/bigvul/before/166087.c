void FileSystemManagerImpl::Remove(const GURL& path,
                                   bool recursive,
                                   RemoveCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  FileSystemURL url(context_->CrackURL(path));
  base::Optional<base::File::Error> opt_error = ValidateFileSystemURL(url);
  if (opt_error) {
    std::move(callback).Run(opt_error.value());
    return;
  }
  if (!security_policy_->CanDeleteFileSystemFile(process_id_, url)) {
    std::move(callback).Run(base::File::FILE_ERROR_SECURITY);
    return;
  }

  operation_runner()->Remove(
      url, recursive,
      base::BindRepeating(&FileSystemManagerImpl::DidFinish, GetWeakPtr(),
                          base::Passed(&callback)));
}
