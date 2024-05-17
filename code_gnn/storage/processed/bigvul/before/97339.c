void FrameLoader::addHTTPOriginIfNeeded(ResourceRequest& request, String origin)
{
    if (!request.httpOrigin().isEmpty())
        return;   

    if (request.httpMethod() == "GET" || request.httpMethod() == "HEAD")
        return;


    if (origin.isEmpty()) {
        origin = SecurityOrigin::createEmpty()->toString();
    }

    request.setHTTPOrigin(origin);
}
