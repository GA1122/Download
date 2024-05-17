void FrameLoader::setReferrerForFrameRequest(ResourceRequest& request, ShouldSendReferrer shouldSendReferrer, Document* originDocument)
{
    if (!originDocument)
        return;
    if (request.didSetHTTPReferrer())
        return;
    if (shouldSendReferrer == NeverSendReferrer)
        return;

    Referrer referrer = SecurityPolicy::generateReferrer(originDocument->getReferrerPolicy(), request.url(), originDocument->outgoingReferrer());

    request.setHTTPReferrer(referrer);
    RefPtr<SecurityOrigin> referrerOrigin = SecurityOrigin::createFromString(referrer.referrer);
    request.addHTTPOriginIfNeeded(referrerOrigin);
}
