int SocketStream::DoResolveProxy() {
  DCHECK(context_);
  DCHECK(!pac_request_);
  next_state_ = STATE_RESOLVE_PROXY_COMPLETE;

  if (!proxy_url_.is_valid()) {
    next_state_ = STATE_CLOSE;
    return ERR_INVALID_ARGUMENT;
  }


  return context_->proxy_service()->ResolveProxy(
      proxy_url_, &proxy_info_, io_callback_, &pac_request_, net_log_);
}
