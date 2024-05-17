PreconnectManager::PreconnectManager(base::WeakPtr<Delegate> delegate,
                                     Profile* profile)
    : delegate_(std::move(delegate)),
      profile_(profile),
      inflight_preresolves_count_(0) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  DCHECK(profile_);
}