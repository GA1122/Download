void ResourceFetcher::determineTargetType(ResourceRequest& request, Resource::Type type)
{
    ResourceRequest::TargetType targetType = requestTargetType(this, request, type);
    request.setTargetType(targetType);
}
