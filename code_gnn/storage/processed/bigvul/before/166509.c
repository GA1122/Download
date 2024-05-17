void ChromeContentBrowserClient::HandleServiceRequest(
    const std::string& service_name,
    service_manager::mojom::ServiceRequest request) {
#if BUILDFLAG(ENABLE_MOJO_MEDIA_IN_BROWSER_PROCESS)
  if (service_name == media::mojom::kMediaServiceName) {
    service_manager::Service::RunUntilTermination(
        media::CreateMediaService(std::move(request)));
  }
#endif
}
