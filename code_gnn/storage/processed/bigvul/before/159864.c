void TopSitesImpl::AddBlacklistedURL(const GURL& url) {
  DCHECK(thread_checker_.CalledOnValidThread());

  auto dummy = base::MakeUnique<base::Value>();
  {
    DictionaryPrefUpdate update(pref_service_, kMostVisitedURLsBlacklist);
    base::DictionaryValue* blacklist = update.Get();
    blacklist->SetWithoutPathExpansion(GetURLHash(url), std::move(dummy));
  }

  ResetThreadSafeCache();
  NotifyTopSitesChanged(TopSitesObserver::ChangeReason::BLACKLIST);
}
