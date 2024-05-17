    void GotAllCookies() {
      net::CookieList master_cookie_list;
      for (const auto& pair : cookies_)
        master_cookie_list.push_back(pair.second);

      BrowserThread::PostTask(
          BrowserThread::UI, FROM_HERE,
          base::BindOnce(&CookieRetriever::SendCookiesResponseOnUI, this,
                         master_cookie_list));
    }
