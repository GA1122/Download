void RequestSender::Send(
    const std::vector<GURL>& urls,
    const base::flat_map<std::string, std::string>& request_extra_headers,
    const std::string& request_body,
    bool use_signing,
    RequestSenderCallback request_sender_callback) {
  DCHECK(thread_checker_.CalledOnValidThread());

  urls_ = urls;
  request_extra_headers_ = request_extra_headers;
  request_body_ = request_body;
  use_signing_ = use_signing;
  request_sender_callback_ = std::move(request_sender_callback);

  if (urls_.empty()) {
    return HandleSendError(static_cast<int>(ProtocolError::MISSING_URLS), 0);
  }

  cur_url_ = urls_.begin();

  if (use_signing_) {
    public_key_ = GetKey(kKeyPubBytesBase64);
    if (public_key_.empty())
      return HandleSendError(
          static_cast<int>(ProtocolError::MISSING_PUBLIC_KEY), 0);
  }

  SendInternal();
}
