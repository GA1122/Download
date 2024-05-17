ServiceWorkerContextCore::ServiceWorkerContextCore(
    const base::FilePath& user_data_directory,
    scoped_refptr<base::SequencedTaskRunner> database_task_runner,
    storage::QuotaManagerProxy* quota_manager_proxy,
    storage::SpecialStoragePolicy* special_storage_policy,
    URLLoaderFactoryGetter* url_loader_factory_getter,
    base::ObserverListThreadSafe<ServiceWorkerContextCoreObserver>*
        observer_list,
    ServiceWorkerContextWrapper* wrapper)
    : wrapper_(wrapper),
      providers_(std::make_unique<ProviderByIdMap>()),
      provider_by_uuid_(std::make_unique<ProviderByClientUUIDMap>()),
      loader_factory_getter_(url_loader_factory_getter),
      force_update_on_page_load_(false),
      was_service_worker_registered_(false),
      observer_list_(observer_list),
      weak_factory_(this) {
  DCHECK(observer_list_);
  storage_ = ServiceWorkerStorage::Create(
      user_data_directory, AsWeakPtr(), std::move(database_task_runner),
      quota_manager_proxy, special_storage_policy);
  job_coordinator_ = std::make_unique<ServiceWorkerJobCoordinator>(AsWeakPtr());
}
