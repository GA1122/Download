ResourceDispatcherHostImpl::MaybeInterceptAsStream(
    net::URLRequest* request,
    network::ResourceResponse* response,
    std::string* payload) {
  payload->clear();
  const std::string& mime_type = response->head.mime_type;

  GURL origin;
  if (!delegate_ || !delegate_->ShouldInterceptResourceAsStream(
                        request, mime_type, &origin, payload)) {
    return nullptr;
  }

  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);
  StreamContext* stream_context =
      GetStreamContextForResourceContext(info->GetContext());

  auto handler = std::make_unique<StreamResourceHandler>(
      request, stream_context->registry(), origin, false);

  info->set_is_stream(true);
  auto stream_info = std::make_unique<StreamInfo>();
  stream_info->handle = handler->stream()->CreateHandle();
  stream_info->original_url = request->url();
  stream_info->mime_type = mime_type;
  if (response->head.headers.get()) {
    stream_info->response_headers =
        base::MakeRefCounted<net::HttpResponseHeaders>(
            response->head.headers->raw_headers());
  }
  delegate_->OnStreamCreated(request, std::move(stream_info));
  return std::move(handler);
}
