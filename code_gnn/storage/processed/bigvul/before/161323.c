void DeleteCookiesOnIO(net::URLRequestContextGetter* context_getter,
                       const std::string& name,
                       const std::string& url,
                       const std::string& domain,
                       const std::string& path,
                       base::OnceClosure callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  net::URLRequestContext* request_context =
      context_getter->GetURLRequestContext();

  request_context->cookie_store()->GetAllCookiesAsync(base::BindOnce(
      &DeleteSelectedCookiesOnIO, base::Unretained(context_getter), name, url,
      domain, path, std::move(callback)));
}
