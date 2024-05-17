void CookiesSetOnIO(std::unique_ptr<SetCookiesCallback> callback,
                    bool success) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::BindOnce(&SetCookiesCallback::sendSuccess, std::move(callback)));
}
