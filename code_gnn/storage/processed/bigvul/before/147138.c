void DocumentLoader::StartLoading() {
  GetTiming().MarkNavigationStart();
  DCHECK(!main_resource_);
  DCHECK_EQ(state_, kNotStarted);
  state_ = kProvisional;

  if (MaybeLoadEmpty())
    return;

  DCHECK(GetTiming().NavigationStart());

  if (!frame_->GetSettings()->GetBrowserSideNavigationEnabled()) {
    DCHECK(!GetTiming().FetchStart());
    GetTiming().MarkFetchStart();
  }

  ResourceLoaderOptions options;
  options.data_buffering_policy = kDoNotBufferData;
  options.initiator_info.name = FetchInitiatorTypeNames::document;
  FetchParameters fetch_params(request_, options);
  main_resource_ =
      RawResource::FetchMainResource(fetch_params, Fetcher(), substitute_data_);

  if (!main_resource_ ||
      (frame_->GetSettings()->GetBrowserSideNavigationEnabled() &&
       main_resource_->ErrorOccurred())) {
    request_ = ResourceRequest(BlankURL());
    MaybeLoadEmpty();
    return;
  }
  request_ = main_resource_->IsLoading() ? main_resource_->GetResourceRequest()
                                         : fetch_params.GetResourceRequest();
  main_resource_->AddClient(this);
}
