void CreateResourceUsageReporter(
    chrome::mojom::ResourceUsageReporterRequest request) {
  mojo::MakeStrongBinding(base::MakeUnique<ResourceUsageReporterImpl>(),
                          std::move(request));
}
