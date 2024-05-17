ResourceDispatcherHostImpl::HttpAuthRelationTypeOf(
    const GURL& request_url,
    const GURL& first_party) {
  if (!first_party.is_valid())
    return HTTP_AUTH_RELATION_TOP;

  if (net::registry_controlled_domains::SameDomainOrHost(
          first_party, request_url,
          net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES)) {
    if (!allow_cross_origin_auth_prompt() && IsOriginSecure(first_party) &&
        !IsOriginSecure(request_url)) {
      return HTTP_AUTH_RELATION_BLOCKED_CROSS;
    }
    return HTTP_AUTH_RELATION_SAME_DOMAIN;
  }

  if (allow_cross_origin_auth_prompt())
    return HTTP_AUTH_RELATION_ALLOWED_CROSS;

  return HTTP_AUTH_RELATION_BLOCKED_CROSS;
}
