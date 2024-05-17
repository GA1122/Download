static inline void dumpErrorDescriptionSuitableForTestResult(WKErrorRef error, StringBuilder& stringBuilder)
{
    WKRetainPtr<WKStringRef> errorDomain = adoptWK(WKErrorCopyDomain(error));
    int errorCode = WKErrorGetErrorCode(error);

    if (WKStringIsEqualToUTF8CString(errorDomain.get(), "WebKitNetworkError")) {
        errorDomain = adoptWK(WKStringCreateWithUTF8CString("NSURLErrorDomain"));
        errorCode = -999;
    }

    if (WKStringIsEqualToUTF8CString(errorDomain.get(), "WebKitPolicyError"))
        errorDomain = adoptWK(WKStringCreateWithUTF8CString("WebKitErrorDomain"));

    stringBuilder.appendLiteral("<NSError domain ");
    stringBuilder.append(toWTFString(errorDomain));
    stringBuilder.appendLiteral(", code ");
    stringBuilder.appendNumber(errorCode);

    WKRetainPtr<WKURLRef> url = adoptWK(WKErrorCopyFailingURL(error));
    if (url.get()) {
        WKRetainPtr<WKStringRef> urlString = adoptWK(WKURLCopyString(url.get()));
        stringBuilder.appendLiteral(", failing URL \"");
        stringBuilder.append(toWTFString(urlString));
        stringBuilder.append('"');
    }

    stringBuilder.append('>');
}
