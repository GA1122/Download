void WebLocalFrameImpl::SetSharedWorkerRepositoryClient(
    WebSharedWorkerRepositoryClient* client) {
  shared_worker_repository_client_ =
      SharedWorkerRepositoryClientImpl::Create(client);
}
