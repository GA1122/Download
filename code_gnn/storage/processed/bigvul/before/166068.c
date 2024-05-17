void FileSystemManagerImpl::DidResolveURL(
    ResolveURLCallback callback,
    base::File::Error result,
    const storage::FileSystemInfo& info,
    const base::FilePath& file_path,
    storage::FileSystemContext::ResolvedEntryType type) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (result == base::File::FILE_OK &&
      type == storage::FileSystemContext::RESOLVED_ENTRY_NOT_FOUND)
    result = base::File::FILE_ERROR_NOT_FOUND;

  base::FilePath normalized_path(
      storage::VirtualPath::GetNormalizedFilePath(file_path));
  std::move(callback).Run(
      mojo::ConvertTo<blink::mojom::FileSystemInfoPtr>(info),
      std::move(normalized_path),
      type == storage::FileSystemContext::RESOLVED_ENTRY_DIRECTORY, result);
}
