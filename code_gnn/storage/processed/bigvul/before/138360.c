  void AddEmbeddedService(const std::string& name,
                          const service_manager::EmbeddedServiceInfo& info) {
    io_task_runner_->PostTask(
        FROM_HERE, base::Bind(&ServiceManagerConnectionImpl::IOThreadContext::
                                  AddEmbeddedServiceRequestHandlerOnIoThread,
                              this, name, info));
  }
