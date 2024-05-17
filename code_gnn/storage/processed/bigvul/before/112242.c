void UrlFetcher::Core::CallCallback(const net::URLRequestStatus& status,
                                    int response_code) {
  DCHECK(delegate_message_loop_->BelongsToCurrentThread());
  if (!done_callback_.is_null()) {
    done_callback_.Run(status, response_code, data_);
  }
}
