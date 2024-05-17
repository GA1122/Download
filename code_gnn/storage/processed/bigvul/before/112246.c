void UrlFetcher::Core::DoStart() {
  DCHECK(network_task_runner_->BelongsToCurrentThread());

  request_.reset(new net::URLRequest(
      url_, this, request_context_getter_->GetURLRequestContext()));

  switch (method_) {
    case GET:
      break;

    case POST:
      DCHECK(!upload_content_.empty());
      DCHECK(!upload_content_type_.empty());
      request_->set_method("POST");

      request_headers_.SetHeader(net::HttpRequestHeaders::kContentType,
                                 upload_content_type_);

      request_->AppendBytesToUpload(
          upload_content_.data(), static_cast<int>(upload_content_.length()));
      break;
  }

  request_->SetExtraRequestHeaders(request_headers_);

  request_->Start();
}
