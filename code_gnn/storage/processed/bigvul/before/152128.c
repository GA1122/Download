  void DidGetAllRegistrations(
      const base::WeakPtr<ServiceWorkerContextCore>& context,
      blink::ServiceWorkerStatusCode status,
      const std::vector<ServiceWorkerRegistrationInfo>& registrations) {
    if (!context || status != blink::ServiceWorkerStatusCode::kOk)
      return;
    const std::map<int64_t, ServiceWorkerVersion*> live_versions_copy =
        context->GetLiveVersions();
    for (const auto& version_itr : live_versions_copy) {
      ServiceWorkerVersion* version(version_itr.second);
      if (version->running_status() == EmbeddedWorkerStatus::STARTING ||
          version->running_status() == EmbeddedWorkerStatus::RUNNING) {
        version->StopWorker(base::DoNothing());
      }
    }
    for (const auto& registration_info : registrations) {
      context->UnregisterServiceWorker(
          registration_info.scope,
          base::BindOnce(&ClearAllServiceWorkersHelper::OnResult, this));
    }
  }
