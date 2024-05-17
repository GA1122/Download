void ClearedCookiesOnIO(std::unique_ptr<ClearBrowserCookiesCallback> callback,
                        uint32_t num_deleted) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::BindOnce(&ClearBrowserCookiesCallback::sendSuccess,
                     std::move(callback)));
}
