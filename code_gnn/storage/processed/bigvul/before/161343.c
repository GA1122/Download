    void RetrieveAllCookiesOnIO(
        net::URLRequestContextGetter* context_getter) {
      DCHECK_CURRENTLY_ON(BrowserThread::IO);
      callback_count_ = 1;

      net::URLRequestContext* request_context =
          context_getter->GetURLRequestContext();
      request_context->cookie_store()->GetAllCookiesAsync(
          base::BindOnce(&CookieRetriever::GotCookies, this));
    }
