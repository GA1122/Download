TabContentsWrapper* DefaultTabHandler::CreateTabContentsForURL(
    const GURL& url,
    const GURL& referrer,
    Profile* profile,
    PageTransition::Type transition,
    bool defer_load,
    SiteInstance* instance) const {
  return delegate_->AsBrowser()->CreateTabContentsForURL(url,
                                                         referrer,
                                                         profile,
                                                         transition,
                                                         defer_load,
                                                         instance);
}
