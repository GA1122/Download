void UrlFetcher::Core::OnReadCompleted(net::URLRequest* request,
                                       int bytes_read) {
  DCHECK_EQ(request, request_.get());
  DCHECK(network_task_runner_->BelongsToCurrentThread());

  do {
    if (!request_->status().is_success() || bytes_read <= 0)
      break;

    data_.append(buffer_->data(), bytes_read);
  } while (request_->Read(buffer_, kBufferSize, &bytes_read));

  const net::URLRequestStatus status = request_->status();
  if (!status.is_io_pending()) {
    delegate_message_loop_->PostTask(
        FROM_HERE, base::Bind(&UrlFetcher::Core::CallCallback, this, status,
                              request_->GetResponseCode()));
  }
}
