void CreateSingleSampleMetricsProvider(
    scoped_refptr<base::SingleThreadTaskRunner> task_runner,
    service_manager::Connector* connector,
    metrics::mojom::SingleSampleMetricsProviderRequest request) {
  if (task_runner->BelongsToCurrentThread()) {
    connector->BindInterface(mojom::kBrowserServiceName, std::move(request));
    return;
  }

  task_runner->PostTask(
      FROM_HERE,
      base::BindOnce(&CreateSingleSampleMetricsProvider, std::move(task_runner),
                     connector, base::Passed(&request)));
}
