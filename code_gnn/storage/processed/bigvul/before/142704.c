ResourceRequest FrameLoader::resourceRequestFromHistoryItem(HistoryItem* item, WebCachePolicy cachePolicy)
{
    RefPtr<EncodedFormData> formData = item->formData();
    ResourceRequest request(item->url());
    request.setHTTPReferrer(item->referrer());
    request.setCachePolicy(cachePolicy);
    if (formData) {
        request.setHTTPMethod(HTTPNames::POST);
        request.setHTTPBody(formData);
        request.setHTTPContentType(item->formContentType());
        RefPtr<SecurityOrigin> securityOrigin = SecurityOrigin::createFromString(item->referrer().referrer);
        request.addHTTPOriginIfNeeded(securityOrigin);
    }
    return request;
}
