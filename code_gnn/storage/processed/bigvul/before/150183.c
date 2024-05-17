  void MaybeActivateProfile() {
    if (!profile_to_activate_)
      return;
    auto i = launched_profiles_.begin();
    for (; i != launched_profiles_.end(); ++i) {
      if (opened_profiles_.find(*i) == opened_profiles_.end())
        return;
    }
    base::PostTask(FROM_HERE, {BrowserThread::UI},
                   base::BindOnce(&ProfileLaunchObserver::ActivateProfile,
                                  base::Unretained(this)));
    registrar_.RemoveAll();
    BrowserList::RemoveObserver(this);
  }
