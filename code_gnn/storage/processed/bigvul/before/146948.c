std::unique_ptr<WebURLLoader> WebLocalFrameImpl::CreateURLLoader(
    const WebURLRequest& request,
    SingleThreadTaskRunner* task_runner) {
  return client_->CreateURLLoader(request, task_runner);
}
