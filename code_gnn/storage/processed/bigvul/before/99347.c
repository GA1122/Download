  virtual void RunWithParams(const Tuple1<int>& params) {

    net::CookieMonster* cookie_monster =
        context_->cookie_store()->GetCookieMonster();
    net::CookieMonster::CookieList cookie_list =
        cookie_monster->GetAllCookiesForURL(url_);

    std::vector<webkit_glue::WebCookie> cookies;
    for (size_t i = 0; i < cookie_list.size(); ++i) {
      cookies.push_back(
          webkit_glue::WebCookie(cookie_list[i].first, cookie_list[i].second));
    }

    ViewHostMsg_GetRawCookies::WriteReplyParams(reply_msg_, cookies);
    filter_->Send(reply_msg_);
    delete this;
  }
