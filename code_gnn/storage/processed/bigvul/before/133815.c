  void OnGotTabRedirects(const GURL& url,
                         const history::RedirectList* redirect_list) {
    DCHECK_CURRENTLY_ON(BrowserThread::UI);
    DCHECK_EQ(url, tab_url_);
    if (!service_)
      return;

    if (!redirect_list->empty()) {
      tab_redirects_.insert(
          tab_redirects_.end(), redirect_list->rbegin(), redirect_list->rend());
    }

    SendRequest();
  }
