WebContext::BrowserContextDelegate::GetIOClient() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::IO);
  base::AutoLock lock(io_client_lock_);
  return io_client_.toStrongRef();
}
