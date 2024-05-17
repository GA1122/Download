void ServiceWorkerContextCore::AddLiveVersion(ServiceWorkerVersion* version) {
  CHECK(!GetLiveVersion(version->version_id()));
  live_versions_[version->version_id()] = version;
  version->AddObserver(this);
  ServiceWorkerVersionInfo version_info = version->GetInfo();
  observer_list_->Notify(FROM_HERE,
                         &ServiceWorkerContextCoreObserver::OnNewLiveVersion,
                         version_info);
}
