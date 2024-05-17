bool ResourceDispatcherHostImpl::DoNotPromptForLogin(
    ResourceType resource_type,
    const GURL& url,
    const GURL& site_for_cookies) {
  if (resource_type == RESOURCE_TYPE_IMAGE &&
      HTTP_AUTH_RELATION_BLOCKED_CROSS ==
          HttpAuthRelationTypeOf(url, site_for_cookies)) {
    return true;
  }
  return false;
}
