ResourceDispatcherHostImpl::HttpAuthResourceTypeOf(net::URLRequest* request) {
  if (!request->first_party_for_cookies().is_valid())
    return HTTP_AUTH_RESOURCE_TOP;

  if (net::RegistryControlledDomainService::SameDomainOrHost(
          request->first_party_for_cookies(), request->url()))
    return HTTP_AUTH_RESOURCE_SAME_DOMAIN;

  if (allow_cross_origin_auth_prompt())
    return HTTP_AUTH_RESOURCE_ALLOWED_CROSS;

  return HTTP_AUTH_RESOURCE_BLOCKED_CROSS;
}
