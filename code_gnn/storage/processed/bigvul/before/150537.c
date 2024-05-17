  bool WasUrlPathMonitored(const std::string& url_path) {
    base::AutoLock lock(lock_);
    for (const auto monitored_url : monitored_urls_)
      if (monitored_url.path() == url_path)
        return true;

    return false;
  }
