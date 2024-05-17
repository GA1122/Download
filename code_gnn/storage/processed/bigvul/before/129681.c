bool ResourceFetcher::canAccessResource(Resource* resource, SecurityOrigin* sourceOrigin, const KURL& url) const
{
    if (!canRequest(resource->type(), url, resource->options(), false, FetchRequest::UseDefaultOriginRestrictionForType))
        return false;

    if (!sourceOrigin && document())
        sourceOrigin = document()->securityOrigin();

    if (sourceOrigin->canRequest(url))
        return true;

    String errorDescription;
    if (!resource->passesAccessControlCheck(sourceOrigin, errorDescription)) {
        if (frame() && frame()->document()) {
            String resourceType = Resource::resourceTypeToString(resource->type(), resource->options().initiatorInfo);
            frame()->document()->addConsoleMessage(JSMessageSource, ErrorMessageLevel, resourceType + " from origin '" + SecurityOrigin::create(url)->toString() + "' has been blocked from loading by Cross-Origin Resource Sharing policy: " + errorDescription);
        }
        return false;
    }
    return true;
}
