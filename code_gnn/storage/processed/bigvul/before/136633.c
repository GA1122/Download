void DocumentLoader::StartLoading() {
  GetTiming().MarkNavigationStart();
  DCHECK(!GetResource());
  DCHECK_EQ(state_, kNotStarted);
  state_ = kProvisional;

  if (MaybeLoadEmpty())
    return;

  DCHECK(!GetTiming().NavigationStart().is_null());

  if (!frame_->GetSettings()->GetBrowserSideNavigationEnabled()) {
    DCHECK(GetTiming().FetchStart().is_null());
    GetTiming().MarkFetchStart();
  }

  ResourceLoaderOptions options;
  options.data_buffering_policy = kDoNotBufferData;
  options.initiator_info.name = FetchInitiatorTypeNames::document;
  FetchParameters fetch_params(request_, options);
  RawResource::FetchMainResource(fetch_params, Fetcher(), this,
                                 substitute_data_);
  request_ = GetResource()->IsLoading() ? GetResource()->GetResourceRequest()
                                        : fetch_params.GetResourceRequest();
}
