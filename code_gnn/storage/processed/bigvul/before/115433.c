static inline void dumpResponseDescriptionSuitableForTestResult(WKURLResponseRef response, StringBuilder& stringBuilder)
{
    WKRetainPtr<WKURLRef> url = adoptWK(WKURLResponseCopyURL(response));
    if (!url) {
        stringBuilder.appendLiteral("(null)");
        return;
    }
    stringBuilder.appendLiteral("<NSURLResponse ");
    stringBuilder.append(pathSuitableForTestResult(url.get()));
    stringBuilder.appendLiteral(", http status code ");
    stringBuilder.appendNumber(WKURLResponseHTTPStatusCode(response));
    stringBuilder.append('>');
}
