void QuicClientPromisedInfo::OnResponseHeaders(const SpdyHeaderBlock& headers) {
  response_headers_.reset(new SpdyHeaderBlock(headers.Clone()));
  if (client_request_delegate_) {
    FinalValidation();
  }
}
