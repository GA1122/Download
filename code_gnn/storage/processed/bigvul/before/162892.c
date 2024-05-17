void ProfilingService::OnProfilingServiceRequest(
    service_manager::ServiceContextRefFactory* ref_factory,
    mojom::ProfilingServiceRequest request) {
  binding_set_.AddBinding(this, std::move(request));
}
