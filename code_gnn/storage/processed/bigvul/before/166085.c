void FileSystemManagerImpl::ReadMetadata(const GURL& path,
                                         ReadMetadataCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  FileSystemURL url(context_->CrackURL(path));
  base::Optional<base::File::Error> opt_error = ValidateFileSystemURL(url);
  if (opt_error) {
    std::move(callback).Run(base::File::Info(), opt_error.value());
    return;
  }
  if (!security_policy_->CanReadFileSystemFile(process_id_, url)) {
    std::move(callback).Run(base::File::Info(),
                            base::File::FILE_ERROR_SECURITY);
    return;
  }

  operation_runner()->GetMetadata(
      url,
      FileSystemOperation::GET_METADATA_FIELD_IS_DIRECTORY |
          FileSystemOperation::GET_METADATA_FIELD_SIZE |
          FileSystemOperation::GET_METADATA_FIELD_LAST_MODIFIED,
      base::BindRepeating(&FileSystemManagerImpl::DidGetMetadata, GetWeakPtr(),
                          base::Passed(&callback)));
}
