base::WeakPtr<FileSystemManagerImpl> FileSystemManagerImpl::GetWeakPtr() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  return weak_factory_.GetWeakPtr();
}
