base::Optional<base::File::Error> FileSystemManagerImpl::ValidateFileSystemURL(
    const storage::FileSystemURL& url) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (!FileSystemURLIsValid(context_, url))
    return base::File::FILE_ERROR_INVALID_URL;

  if (url.type() == storage::kFileSystemTypePluginPrivate)
    return base::File::FILE_ERROR_SECURITY;

  return base::nullopt;
}
