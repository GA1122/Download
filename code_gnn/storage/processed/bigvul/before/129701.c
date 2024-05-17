ResourcePtr<CSSStyleSheetResource> ResourceFetcher::fetchCSSStyleSheet(FetchRequest& request)
{
    return toCSSStyleSheetResource(requestResource(Resource::CSSStyleSheet, request));
}
