void DownloadResourceHandler::OnResponseCompleted(
    const net::URLRequestStatus& status,
    std::unique_ptr<ResourceController> controller) {
  core_.OnResponseCompleted(status);
  controller->Resume();
}
