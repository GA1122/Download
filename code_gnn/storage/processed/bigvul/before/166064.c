void FileSystemManagerImpl::DidOpenFileSystem(
    OpenCallback callback,
    const GURL& root,
    const std::string& filesystem_name,
    base::File::Error result) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(root.is_valid() || result != base::File::FILE_OK);
  std::move(callback).Run(filesystem_name, root, result);
}
