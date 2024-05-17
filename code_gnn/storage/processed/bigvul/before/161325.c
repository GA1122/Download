void DeletedCookiesOnIO(base::OnceClosure callback, uint32_t num_deleted) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  BrowserThread::PostTask(BrowserThread::UI, FROM_HERE, std::move(callback));
}
