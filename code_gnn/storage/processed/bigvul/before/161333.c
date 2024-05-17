    void GotCookies(const net::CookieList& cookie_list) {
      DCHECK_CURRENTLY_ON(BrowserThread::IO);
      for (const net::CanonicalCookie& cookie : cookie_list) {
        std::string key = base::StringPrintf(
            "%s::%s::%s::%d", cookie.Name().c_str(), cookie.Domain().c_str(),
            cookie.Path().c_str(), cookie.IsSecure());
        cookies_[key] = cookie;
      }

      --callback_count_;
      if (callback_count_ == 0)
        GotAllCookies();
    }
