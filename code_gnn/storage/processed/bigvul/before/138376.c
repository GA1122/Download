ServiceManagerConnectionImpl::ServiceManagerConnectionImpl(
    service_manager::mojom::ServiceRequest request,
    scoped_refptr<base::SequencedTaskRunner> io_task_runner)
    : weak_factory_(this) {
  service_manager::mojom::ConnectorRequest connector_request;
  connector_ = service_manager::Connector::Create(&connector_request);

  std::unique_ptr<service_manager::Connector> io_thread_connector =
      connector_->Clone();
  context_ = new IOThreadContext(
      std::move(request), io_task_runner, std::move(io_thread_connector),
      std::move(connector_request));
}
