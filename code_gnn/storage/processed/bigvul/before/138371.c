  IOThreadContext(
      service_manager::mojom::ServiceRequest service_request,
      scoped_refptr<base::SequencedTaskRunner> io_task_runner,
      std::unique_ptr<service_manager::Connector> io_thread_connector,
      service_manager::mojom::ConnectorRequest connector_request)
      : pending_service_request_(std::move(service_request)),
        io_task_runner_(io_task_runner),
        io_thread_connector_(std::move(io_thread_connector)),
        pending_connector_request_(std::move(connector_request)),
        child_binding_(this),
        weak_factory_(this) {
    io_thread_checker_.DetachFromThread();
  }
