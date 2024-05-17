void ClearCookiesOnIO(net::URLRequestContextGetter* context_getter,
                      std::unique_ptr<ClearBrowserCookiesCallback> callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  net::URLRequestContext* request_context =
      context_getter->GetURLRequestContext();
  request_context->cookie_store()->DeleteAllAsync(
      base::BindOnce(&ClearedCookiesOnIO, base::Passed(std::move(callback))));
}
