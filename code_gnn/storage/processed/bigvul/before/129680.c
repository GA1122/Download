bool ResourceFetcher::canAccessRedirect(Resource* resource, ResourceRequest& request, const ResourceResponse& redirectResponse, ResourceLoaderOptions& options)
{
    if (!canRequest(resource->type(), request.url(), options, false, FetchRequest::UseDefaultOriginRestrictionForType))
        return false;
    if (options.corsEnabled == IsCORSEnabled) {
        SecurityOrigin* sourceOrigin = options.securityOrigin.get();
        if (!sourceOrigin && document())
            sourceOrigin = document()->securityOrigin();

        String errorMessage;
        if (!CrossOriginAccessControl::handleRedirect(resource, sourceOrigin, request, redirectResponse, options, errorMessage)) {
            if (frame() && frame()->document())
                frame()->document()->addConsoleMessage(JSMessageSource, ErrorMessageLevel, errorMessage);
            return false;
        }
    }
    if (resource->type() == Resource::Image && shouldDeferImageLoad(request.url()))
        return false;
    return true;
}
