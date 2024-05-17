int SocketStream::DoReadTunnelHeadersComplete(int result) {
  DCHECK_EQ(kTunnelProxy, proxy_mode_);

  if (result < 0) {
    next_state_ = STATE_CLOSE;
    return result;
  }

  if (result == 0) {
    next_state_ = STATE_CLOSE;
    return ERR_CONNECTION_CLOSED;
  }

  tunnel_response_headers_len_ += result;
  DCHECK(tunnel_response_headers_len_ <= tunnel_response_headers_capacity_);

  int eoh = HttpUtil::LocateEndOfHeaders(
      tunnel_response_headers_->headers(), tunnel_response_headers_len_, 0);
  if (eoh == -1) {
    if (tunnel_response_headers_len_ >= kMaxTunnelResponseHeadersSize) {
      next_state_ = STATE_CLOSE;
      return ERR_RESPONSE_HEADERS_TOO_BIG;
    }

    next_state_ = STATE_READ_TUNNEL_HEADERS;
    return OK;
  }
  scoped_refptr<HttpResponseHeaders> headers;
  headers = new HttpResponseHeaders(
      HttpUtil::AssembleRawHeaders(tunnel_response_headers_->headers(), eoh));
  if (headers->GetParsedHttpVersion() < HttpVersion(1, 0)) {
    next_state_ = STATE_CLOSE;
    return ERR_TUNNEL_CONNECTION_FAILED;
  }
  switch (headers->response_code()) {
    case 200:   
      if (is_secure()) {
        DCHECK_EQ(eoh, tunnel_response_headers_len_);
        next_state_ = STATE_SSL_CONNECT;
      } else {
        result = DidEstablishConnection();
        if (result < 0) {
          next_state_ = STATE_CLOSE;
          return result;
        }
        if ((eoh < tunnel_response_headers_len_) && delegate_)
          delegate_->OnReceivedData(
              this, tunnel_response_headers_->headers() + eoh,
              tunnel_response_headers_len_ - eoh);
      }
      return OK;
    case 407:   
      if (proxy_mode_ != kTunnelProxy)
        return ERR_UNEXPECTED_PROXY_AUTH;

      result = proxy_auth_controller_->HandleAuthChallenge(
          headers, false, true, net_log_);
      if (result != OK)
        return result;
      DCHECK(!proxy_info_.is_empty());
      next_state_ = STATE_AUTH_REQUIRED;
      if (proxy_auth_controller_->HaveAuth()) {
        base::MessageLoop::current()->PostTask(
            FROM_HERE, base::Bind(&SocketStream::DoRestartWithAuth, this));
        return ERR_IO_PENDING;
      }
      if (delegate_) {
        base::MessageLoop::current()->PostTask(
            FROM_HERE, base::Bind(&SocketStream::DoAuthRequired, this));
        return ERR_IO_PENDING;
      }
      break;
    default:
      break;
  }
  next_state_ = STATE_CLOSE;
  return ERR_TUNNEL_CONNECTION_FAILED;
}
