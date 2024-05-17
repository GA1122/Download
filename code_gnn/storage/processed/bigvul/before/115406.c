void InjectedBundlePage::didReceiveResponseForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t identifier, WKURLResponseRef response)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    if (InjectedBundle::shared().testRunner()->shouldDumpResourceLoadCallbacks()) {
        StringBuilder stringBuilder;
        dumpResourceURL(identifier, stringBuilder);
        stringBuilder.appendLiteral(" - didReceiveResponse ");
        dumpResponseDescriptionSuitableForTestResult(response, stringBuilder);
        stringBuilder.append('\n');
        InjectedBundle::shared().outputText(stringBuilder.toString());
    }


    if (!InjectedBundle::shared().testRunner()->shouldDumpResourceResponseMIMETypes())
        return;

    WKRetainPtr<WKURLRef> url = adoptWK(WKURLResponseCopyURL(response));
    WKRetainPtr<WKStringRef> urlString = adoptWK(WKURLCopyLastPathComponent(url.get()));
    WKRetainPtr<WKStringRef> mimeTypeString = adoptWK(WKURLResponseCopyMIMEType(response));

    StringBuilder stringBuilder;
    stringBuilder.append(toWTFString(urlString));
    stringBuilder.appendLiteral(" has MIME type ");
    stringBuilder.append(toWTFString(mimeTypeString));
    stringBuilder.append('\n');
    InjectedBundle::shared().outputText(stringBuilder.toString());
}
