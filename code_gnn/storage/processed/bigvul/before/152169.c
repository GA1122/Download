ServiceWorkerContextCore::ServiceWorkerContextCore(
    ServiceWorkerContextCore* old_context,
    ServiceWorkerContextWrapper* wrapper)
    : wrapper_(wrapper),
      providers_(old_context->providers_.release()),
      provider_by_uuid_(old_context->provider_by_uuid_.release()),
      loader_factory_getter_(old_context->loader_factory_getter()),
      was_service_worker_registered_(
          old_context->was_service_worker_registered_),
      observer_list_(old_context->observer_list_),
      next_embedded_worker_id_(old_context->next_embedded_worker_id_),
      weak_factory_(this) {
  DCHECK(observer_list_);

  storage_ = ServiceWorkerStorage::Create(AsWeakPtr(), old_context->storage());
  job_coordinator_ = std::make_unique<ServiceWorkerJobCoordinator>(AsWeakPtr());
}
