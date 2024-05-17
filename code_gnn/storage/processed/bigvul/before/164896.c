void DownloadResourceHandler::OnReadCompleted(
    int bytes_read,
    std::unique_ptr<ResourceController> controller) {
  DCHECK(!has_controller());

  bool defer = false;
  if (!core_.OnReadCompleted(bytes_read, &defer)) {
    controller->Cancel();
    return;
  }

  if (defer) {
    HoldController(std::move(controller));
  } else {
    controller->Resume();
  }
}
