void UrlFetcher::Core::OnResponseStarted(net::URLRequest* request) {
  DCHECK_EQ(request, request_.get());
  DCHECK(network_task_runner_->BelongsToCurrentThread());
  ReadResponse();
}
