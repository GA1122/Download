  explicit ResourceUsageReporterImpl(
      mojo::InterfaceRequest<ResourceUsageReporter> req)
      : binding_(this, req.Pass()) {}
