void Resource::RevalidationSucceeded(
    const ResourceResponse& validating_response) {
  SECURITY_CHECK(redirect_chain_.IsEmpty());
  SECURITY_CHECK(EqualIgnoringFragmentIdentifier(validating_response.Url(),
                                                 GetResponse().Url()));
  response_.SetResourceLoadTiming(validating_response.GetResourceLoadTiming());

  const HTTPHeaderMap& new_headers = validating_response.HttpHeaderFields();
  for (const auto& header : new_headers) {
    if (!ShouldUpdateHeaderAfterRevalidation(header.key))
      continue;
    response_.SetHTTPHeaderField(header.key, header.value);
  }

  is_revalidating_ = false;
}
