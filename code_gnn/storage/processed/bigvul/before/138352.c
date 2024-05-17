  void Start(
      service_manager::mojom::ServicePtrInfo packaged_services_service_info,
      std::unique_ptr<BuiltinManifestProvider> manifest_provider) {
    BrowserThread::GetTaskRunnerForThread(BrowserThread::IO)
        ->PostTask(FROM_HERE,
                   base::Bind(&InProcessServiceManagerContext::StartOnIOThread,
                              this, base::Passed(&manifest_provider),
                              base::Passed(&packaged_services_service_info)));
  }
