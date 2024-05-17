void FetchManager::Loader::PerformDataFetch() {
  DCHECK(fetch_request_data_->Url().ProtocolIsData());

  ResourceRequest request(fetch_request_data_->Url());
  request.SetRequestContext(fetch_request_data_->Context());
  request.SetUseStreamOnResponse(true);
  request.SetHTTPMethod(fetch_request_data_->Method());
  request.SetFetchCredentialsMode(network::mojom::FetchCredentialsMode::kOmit);
  request.SetFetchRedirectMode(FetchRedirectMode::kError);

  ResourceLoaderOptions resource_loader_options;
  resource_loader_options.data_buffering_policy = kDoNotBufferData;
  resource_loader_options.security_origin = fetch_request_data_->Origin().get();

  ThreadableLoaderOptions threadable_loader_options;

  probe::willStartFetch(execution_context_, this);
  threadable_loader_ = ThreadableLoader::Create(*execution_context_, this,
                                                threadable_loader_options,
                                                resource_loader_options);
  threadable_loader_->Start(request);
}
