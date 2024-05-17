  void AddEmbeddedServiceRequestHandlerOnIoThread(
      const std::string& name,
      const service_manager::EmbeddedServiceInfo& info) {
    DCHECK(io_thread_checker_.CalledOnValidThread());
    std::unique_ptr<service_manager::EmbeddedServiceRunner> service(
        new service_manager::EmbeddedServiceRunner(name, info));
    AddServiceRequestHandlerOnIoThread(
        name,
        base::Bind(&service_manager::EmbeddedServiceRunner::BindServiceRequest,
                   base::Unretained(service.get())));
    auto result =
        embedded_services_.insert(std::make_pair(name, std::move(service)));
    DCHECK(result.second);
  }
