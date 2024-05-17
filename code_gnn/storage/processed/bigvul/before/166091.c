void FileSystemManagerImpl::TouchFile(const GURL& path,
                                      base::Time last_access_time,
                                      base::Time last_modified_time,
                                      TouchFileCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  FileSystemURL url(context_->CrackURL(path));
  base::Optional<base::File::Error> opt_error = ValidateFileSystemURL(url);
  if (opt_error) {
    std::move(callback).Run(opt_error.value());
    return;
  }
  if (!security_policy_->CanCreateFileSystemFile(process_id_, url)) {
    std::move(callback).Run(base::File::FILE_ERROR_SECURITY);
    return;
  }

  operation_runner()->TouchFile(
      url, last_access_time, last_modified_time,
      base::BindRepeating(&FileSystemManagerImpl::DidFinish, GetWeakPtr(),
                          base::Passed(&callback)));
}
