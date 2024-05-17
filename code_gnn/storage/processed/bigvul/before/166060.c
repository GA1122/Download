void FileSystemManagerImpl::DidFinish(
    base::OnceCallback<void(base::File::Error)> callback,
    base::File::Error error_code) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  std::move(callback).Run(error_code);
}
