void StartServiceInUtilityProcess(
    const std::string& service_name,
    const base::string16& process_name,
    SandboxType sandbox_type,
    service_manager::mojom::ServiceRequest request) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  UtilityProcessHost* process_host =
      UtilityProcessHost::Create(nullptr, nullptr);
  process_host->SetName(process_name);
  process_host->SetSandboxType(sandbox_type);
  process_host->Start();

  service_manager::mojom::ServiceFactoryPtr service_factory;
  BindInterface(process_host, mojo::MakeRequest(&service_factory));
  service_factory->CreateService(std::move(request), service_name);
}
