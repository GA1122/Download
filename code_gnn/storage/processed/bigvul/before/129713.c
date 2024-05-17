ResourcePtr<XSLStyleSheetResource> ResourceFetcher::fetchXSLStyleSheet(FetchRequest& request)
{
    ASSERT(RuntimeEnabledFeatures::xsltEnabled());
    return toXSLStyleSheetResource(requestResource(Resource::XSLStyleSheet, request));
}
