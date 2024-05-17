    void SendCookiesResponseOnUI(const net::CookieList& cookie_list) {
      DCHECK_CURRENTLY_ON(BrowserThread::UI);
      std::unique_ptr<ProtocolCookieArray> cookies =
          ProtocolCookieArray::create();

      for (const net::CanonicalCookie& cookie : cookie_list) {
        std::unique_ptr<Network::Cookie> devtools_cookie =
            Network::Cookie::Create()
                .SetName(cookie.Name())
                .SetValue(cookie.Value())
                .SetDomain(cookie.Domain())
                .SetPath(cookie.Path())
                .SetExpires(cookie.ExpiryDate().is_null() ? -1 : cookie.ExpiryDate().ToDoubleT())
                .SetSize(cookie.Name().length() + cookie.Value().length())
                .SetHttpOnly(cookie.IsHttpOnly())
                .SetSecure(cookie.IsSecure())
                .SetSession(!cookie.IsPersistent())
                .Build();

        switch (cookie.SameSite()) {
          case net::CookieSameSite::STRICT_MODE:
            devtools_cookie->SetSameSite(Network::CookieSameSiteEnum::Strict);
            break;
          case net::CookieSameSite::LAX_MODE:
            devtools_cookie->SetSameSite(Network::CookieSameSiteEnum::Lax);
            break;
          case net::CookieSameSite::NO_RESTRICTION:
            break;
       }

       cookies->addItem(std::move(devtools_cookie));
      }

      if (callback_) {
        callback_->sendSuccess(std::move(cookies));
      } else {
        DCHECK(all_callback_);
        all_callback_->sendSuccess(std::move(cookies));
      }
    }
