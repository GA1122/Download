  void SetURL(const std::string& url) {
    url_ = GURL(url);
    origin_ = url::Origin::Create(url_);
  }
