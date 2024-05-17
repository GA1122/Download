void ResourceFetcher::HandleLoaderError(Resource* resource,
                                        const ResourceError& error) {
  DCHECK(resource);

  RemoveResourceLoader(resource->Loader());

  resource_timing_info_map_.Take(resource);

  bool is_internal_request = resource->Options().initiator_info.name ==
                             FetchInitiatorTypeNames::internal;

  Context().DispatchDidFail(resource->Identifier(), error,
                            resource->GetResponse().EncodedDataLength(),
                            is_internal_request);

  resource->GetError(error);

  HandleLoadCompletion(resource);
}
