void TopSitesImpl::RemoveBlacklistedURL(const GURL& url) {
  DCHECK(thread_checker_.CalledOnValidThread());
  {
    DictionaryPrefUpdate update(pref_service_, kMostVisitedURLsBlacklist);
    base::DictionaryValue* blacklist = update.Get();
    blacklist->RemoveWithoutPathExpansion(GetURLHash(url), nullptr);
  }
  ResetThreadSafeCache();
  NotifyTopSitesChanged(TopSitesObserver::ChangeReason::BLACKLIST);
}
