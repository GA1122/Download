static void PopulateTimingInfo(ResourceTimingInfo* info, Resource* resource) {
  KURL initial_url = resource->GetResponse().RedirectResponses().IsEmpty()
                         ? resource->GetResourceRequest().Url()
                         : resource->GetResponse().RedirectResponses()[0].Url();
  info->SetInitialURL(initial_url);
  info->SetFinalResponse(resource->GetResponse());
}
