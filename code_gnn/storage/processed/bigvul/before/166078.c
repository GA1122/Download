void FileSystemManagerImpl::Move(const GURL& src_path,
                                 const GURL& dest_path,
                                 MoveCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  FileSystemURL src_url(context_->CrackURL(src_path));
  FileSystemURL dest_url(context_->CrackURL(dest_path));
  base::Optional<base::File::Error> opt_error = ValidateFileSystemURL(src_url);
  if (!opt_error)
    opt_error = ValidateFileSystemURL(dest_url);
  if (opt_error) {
    std::move(callback).Run(opt_error.value());
    return;
  }
  if (!security_policy_->CanReadFileSystemFile(process_id_, src_url) ||
      !security_policy_->CanDeleteFileSystemFile(process_id_, src_url) ||
      !security_policy_->CanCreateFileSystemFile(process_id_, dest_url)) {
    std::move(callback).Run(base::File::FILE_ERROR_SECURITY);
    return;
  }

  operation_runner()->Move(
      src_url, dest_url, storage::FileSystemOperation::OPTION_NONE,
      base::BindRepeating(&FileSystemManagerImpl::DidFinish, GetWeakPtr(),
                          base::Passed(&callback)));
}
