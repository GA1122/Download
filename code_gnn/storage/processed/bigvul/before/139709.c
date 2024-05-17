void AddRedirectsToTimingInfo(Resource* resource, ResourceTimingInfo* info) {
  const auto& responses = resource->GetResponse().RedirectResponses();
  for (size_t i = 0; i < responses.size(); ++i) {
    const KURL& new_url = i + 1 < responses.size()
                              ? KURL(responses[i + 1].Url())
                              : resource->GetResourceRequest().Url();
    bool cross_origin =
        !SecurityOrigin::AreSameSchemeHostPort(responses[i].Url(), new_url);
    info->AddRedirect(responses[i], cross_origin);
  }
}
