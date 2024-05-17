Resource* ResourceFetcher::ResourceForStaticData(
    const FetchParameters& params,
    const ResourceFactory& factory,
    const SubstituteData& substitute_data) {
  const KURL& url = params.GetResourceRequest().Url();
  DCHECK(url.ProtocolIsData() || substitute_data.IsValid() || archive_);

  if (!archive_ && !substitute_data.IsValid() &&
      (factory.GetType() == Resource::kMainResource ||
       factory.GetType() == Resource::kRaw))
    return nullptr;

  const String cache_identifier = GetCacheIdentifier();
  if (Resource* old_resource =
          GetMemoryCache()->ResourceForURL(url, cache_identifier)) {
    if (params.Options().data_buffering_policy != kDoNotBufferData)
      return old_resource;
    GetMemoryCache()->Remove(old_resource);
  }

  ResourceResponse response;
  RefPtr<SharedBuffer> data;
  if (substitute_data.IsValid()) {
    data = substitute_data.Content();
    response.SetURL(url);
    response.SetMimeType(substitute_data.MimeType());
    response.SetExpectedContentLength(data->size());
    response.SetTextEncodingName(substitute_data.TextEncoding());
  } else if (url.ProtocolIsData()) {
    data = NetworkUtils::ParseDataURLAndPopulateResponse(url, response);
    if (!data)
      return nullptr;
  } else {
    ArchiveResource* archive_resource =
        archive_->SubresourceForURL(params.Url());
    if (!archive_resource)
      return nullptr;
    data = archive_resource->Data();
    response.SetURL(url);
    response.SetMimeType(archive_resource->MimeType());
    response.SetExpectedContentLength(data->size());
    response.SetTextEncodingName(archive_resource->TextEncoding());
  }

  Resource* resource = factory.Create(
      params.GetResourceRequest(), params.Options(), params.DecoderOptions());
  resource->SetStatus(ResourceStatus::kPending);
  resource->NotifyStartLoad();
  resource->ResponseReceived(response, nullptr);
  resource->SetDataBufferingPolicy(kBufferData);
  if (data->size())
    resource->SetResourceBuffer(data);
  resource->SetIdentifier(CreateUniqueIdentifier());
  resource->SetCacheIdentifier(cache_identifier);
  resource->Finish();

  if (ShouldResourceBeAddedToMemoryCache(params, resource) &&
      !substitute_data.IsValid()) {
    GetMemoryCache()->Add(resource);
  }

  return resource;
}
