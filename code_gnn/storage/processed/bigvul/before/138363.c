  void AddServiceRequestHandler(const std::string& name,
                                const ServiceRequestHandler& handler) {
    io_task_runner_->PostTask(
        FROM_HERE, base::Bind(&ServiceManagerConnectionImpl::IOThreadContext::
                                  AddServiceRequestHandlerOnIoThread,
                              this, name, handler));
  }
