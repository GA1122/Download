MemoryInstrumentation::MemoryInstrumentation(
    service_manager::Connector* connector,
    const std::string& service_name)
    : connector_(connector),
      connector_task_runner_(base::ThreadTaskRunnerHandle::Get()),
      tls_coordinator_(&DestroyCoordinatorTLS),
      service_name_(service_name) {
  DCHECK(connector_task_runner_);
}
