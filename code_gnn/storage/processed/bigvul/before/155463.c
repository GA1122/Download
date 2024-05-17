void StartDownloadManager(service_manager::mojom::ServiceRequest request) {
  DownloadManagerService::GetInstance()->BindServiceRequest(std::move(request));
}
