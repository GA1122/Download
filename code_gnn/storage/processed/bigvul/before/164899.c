void DownloadResourceHandler::OnResponseStarted(
    network::ResourceResponse* response,
    std::unique_ptr<ResourceController> controller) {
  if (core_.OnResponseStarted(response->head.mime_type)) {
    controller->Resume();
  } else {
    controller->Cancel();
  }
}
