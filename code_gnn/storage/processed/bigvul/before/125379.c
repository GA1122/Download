void GDataFileSystem::Initialize() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  documents_service_->Initialize(profile_);

  directory_service_.reset(new GDataDirectoryService);
  feed_loader_.reset(new GDataWapiFeedLoader(directory_service_.get(),
                                             documents_service_,
                                             webapps_registry_,
                                             cache_,
                                             blocking_task_runner_));
  feed_loader_->AddObserver(this);

  PrefService* pref_service = profile_->GetPrefs();
  hide_hosted_docs_ = pref_service->GetBoolean(prefs::kDisableGDataHostedFiles);

  InitializePreferenceObserver();
}
