    CookieRetriever(std::unique_ptr<GetCookiesCallback> callback)
        : callback_(std::move(callback)),
          all_callback_(nullptr) {}
