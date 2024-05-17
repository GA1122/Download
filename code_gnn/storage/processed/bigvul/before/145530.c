void ResourceDispatcherHostImpl::Shutdown() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  BrowserThread::PostTask(BrowserThread::IO,
                          FROM_HERE,
                          base::Bind(&ResourceDispatcherHostImpl::OnShutdown,
                                     base::Unretained(this)));
}
