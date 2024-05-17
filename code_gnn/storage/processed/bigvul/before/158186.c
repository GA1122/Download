bool IsRedirectSafe(const GURL& from_url,
                    const GURL& to_url,
                    ResourceContext* resource_context) {
  return IsSafeRedirectTarget(from_url, to_url) &&
         GetContentClient()->browser()->IsSafeRedirectTarget(to_url,
                                                             resource_context);
}
