ImageResource* ImageResource::Fetch(FetchParameters& params,
                                    ResourceFetcher* fetcher) {
  if (params.GetResourceRequest().GetRequestContext() ==
      WebURLRequest::kRequestContextUnspecified) {
    params.SetRequestContext(WebURLRequest::kRequestContextImage);
  }

  ImageResource* resource = ToImageResource(
      fetcher->RequestResource(params, ImageResourceFactory(params), nullptr));

  if (params.Options().initiator_info.name == FetchInitiatorTypeNames::uacss)
    resource->FlagAsUserAgentResource();
  return resource;
}
