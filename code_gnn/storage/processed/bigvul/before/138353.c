  void StartOnIOThread(
      std::unique_ptr<BuiltinManifestProvider> manifest_provider,
      service_manager::mojom::ServicePtrInfo packaged_services_service_info) {
    manifest_provider_ = std::move(manifest_provider);
    service_manager_ = base::MakeUnique<service_manager::ServiceManager>(
        base::MakeUnique<NullServiceProcessLauncherFactory>(), nullptr,
        manifest_provider_.get());

    service_manager::mojom::ServicePtr packaged_services_service;
    packaged_services_service.Bind(std::move(packaged_services_service_info));
    service_manager_->RegisterService(
        service_manager::Identity(mojom::kPackagedServicesServiceName,
                                  service_manager::mojom::kRootUserID),
        std::move(packaged_services_service), nullptr);
  }
