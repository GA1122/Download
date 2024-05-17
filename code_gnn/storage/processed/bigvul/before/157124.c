void ResourceMultiBufferDataProvider::Start() {
  DVLOG(1) << __func__ << " @ " << byte_pos();
  if (url_data_->length() > 0 && byte_pos() >= url_data_->length()) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE, base::BindOnce(&ResourceMultiBufferDataProvider::Terminate,
                                  weak_factory_.GetWeakPtr()));
    return;
  }

  auto request = std::make_unique<WebURLRequest>(url_data_->url());
  request->SetRequestContext(is_client_audio_element_
                                 ? blink::mojom::RequestContextType::AUDIO
                                 : blink::mojom::RequestContextType::VIDEO);
  request->SetHTTPHeaderField(
      WebString::FromUTF8(net::HttpRequestHeaders::kRange),
      WebString::FromUTF8(
          net::HttpByteRange::RightUnbounded(byte_pos()).GetHeaderValue()));

  if (url_data_->length() == kPositionNotSpecified &&
      url_data_->CachedSize() == 0 && url_data_->BytesReadFromCache() == 0) {
    request->SetHTTPHeaderField(WebString::FromUTF8("chrome-proxy"),
                                WebString::FromUTF8("frfr"));
  }


  request->SetHTTPHeaderField(
      WebString::FromUTF8(net::HttpRequestHeaders::kAcceptEncoding),
      WebString::FromUTF8("identity;q=1, *;q=0"));

  blink::WebAssociatedURLLoaderOptions options;
  if (url_data_->cors_mode() != UrlData::CORS_UNSPECIFIED) {
    options.expose_all_response_headers = true;
    options.preflight_policy =
        network::mojom::CORSPreflightPolicy::kPreventPreflight;

    request->SetFetchRequestMode(network::mojom::FetchRequestMode::kCORS);
    if (url_data_->cors_mode() != UrlData::CORS_USE_CREDENTIALS) {
      request->SetFetchCredentialsMode(
          network::mojom::FetchCredentialsMode::kSameOrigin);
    }
  }

  active_loader_ =
      url_data_->url_index()->fetch_context()->CreateUrlLoader(options);

  url_data_->WaitToLoad(
      base::BindOnce(&ResourceMultiBufferDataProvider::StartLoading,
                     weak_factory_.GetWeakPtr(), std::move(request)));
}
