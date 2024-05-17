static inline void dumpRequestDescriptionSuitableForTestResult(WKURLRequestRef request, StringBuilder& stringBuilder)
{
    WKRetainPtr<WKURLRef> url = adoptWK(WKURLRequestCopyURL(request));
    WKRetainPtr<WKURLRef> firstParty = adoptWK(WKURLRequestCopyFirstPartyForCookies(request));
    WKRetainPtr<WKStringRef> httpMethod = adoptWK(WKURLRequestCopyHTTPMethod(request));

    stringBuilder.appendLiteral("<NSURLRequest URL ");
    stringBuilder.append(pathSuitableForTestResult(url.get()));
    stringBuilder.appendLiteral(", main document URL ");
    stringBuilder.append(urlSuitableForTestResult(firstParty.get()));
    stringBuilder.appendLiteral(", http method ");

    if (WKStringIsEmpty(httpMethod.get()))
        stringBuilder.appendLiteral("(none)");
    else
        stringBuilder.append(toWTFString(httpMethod));

    stringBuilder.append('>');
}
