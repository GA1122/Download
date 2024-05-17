void FileSystemManagerImpl::DidGetPlatformPath(GetPlatformPathCallback callback,
                                               base::FilePath platform_path) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  std::move(callback).Run(platform_path);
}
