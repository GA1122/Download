    void RetrieveCookiesOnIO(
        net::URLRequestContextGetter* context_getter,
        const std::vector<GURL>& urls) {
      DCHECK_CURRENTLY_ON(BrowserThread::IO);
      callback_count_ = urls.size();

      if (callback_count_ == 0) {
        GotAllCookies();
        return;
      }

      for (const GURL& url : urls) {
        net::URLRequestContext* request_context =
            context_getter->GetURLRequestContext();
        request_context->cookie_store()->GetAllCookiesForURLAsync(
            url, base::BindOnce(&CookieRetriever::GotCookies, this));
      }
    }
