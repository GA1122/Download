void StartServiceInGpuProcess(const std::string& service_name,
                              service_manager::mojom::ServiceRequest request) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  GpuProcessHost* process_host = GpuProcessHost::Get();
  if (!process_host) {
    DLOG(ERROR) << "GPU process host not available.";
    return;
  }

  service_manager::mojom::ServiceFactoryPtr service_factory;
  BindInterfaceInGpuProcess(mojo::MakeRequest(&service_factory));
  service_factory->CreateService(std::move(request), service_name);
}
