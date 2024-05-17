void DownloadResourceHandler::OnWillRead(
    scoped_refptr<net::IOBuffer>* buf,
    int* buf_size,
    std::unique_ptr<ResourceController> controller) {
  if (!core_.OnWillRead(buf, buf_size)) {
    controller->Cancel();
    return;
  }

  controller->Resume();
}
