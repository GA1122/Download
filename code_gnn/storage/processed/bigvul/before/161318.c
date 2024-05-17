    CookieRetriever(std::unique_ptr<GetAllCookiesCallback> callback)
        : callback_(nullptr),
          all_callback_(std::move(callback)) {}
