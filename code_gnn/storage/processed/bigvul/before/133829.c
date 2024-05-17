void CreateResourceUsageReporter(
    mojo::InterfaceRequest<ResourceUsageReporter> request) {
  new ResourceUsageReporterImpl(request.Pass());
}
