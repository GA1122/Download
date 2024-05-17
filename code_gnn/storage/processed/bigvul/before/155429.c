void ChromeContentBrowserClient::RegisterIOThreadServiceHandlers(
    content::ServiceManagerConnection* connection) {
  connection->AddServiceRequestHandler(
      chrome::mojom::kServiceName,
      ChromeService::GetInstance()->CreateChromeServiceRequestHandler());

#if defined(OS_ANDROID)
  connection->AddServiceRequestHandler(
      proxy_resolver::mojom::kProxyResolverServiceName,
      base::BindRepeating([](service_manager::mojom::ServiceRequest request) {
        service_manager::Service::RunAsyncUntilTermination(
            std::make_unique<proxy_resolver::ProxyResolverService>(
                std::move(request)));
      }));
  connection->AddServiceRequestHandler(
      "download_manager", base::BindRepeating(&StartDownloadManager));
#endif

  if (heap_profiling::IsInProcessModeEnabled()) {
    connection->AddServiceRequestHandler(
        heap_profiling::mojom::kServiceName,
        heap_profiling::HeapProfilingService::GetServiceFactory());
  }
}
