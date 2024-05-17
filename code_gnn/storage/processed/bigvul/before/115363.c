WKBundlePagePolicyAction InjectedBundlePage::decidePolicyForResponse(WKBundlePageRef page, WKBundleFrameRef, WKURLResponseRef response, WKURLRequestRef, WKTypeRef*)
{
    if (WKURLResponseIsAttachment(response)) {
        StringBuilder stringBuilder;
        WKRetainPtr<WKStringRef> filename = adoptWK(WKURLResponseCopySuggestedFilename(response));
        stringBuilder.appendLiteral("Policy delegate: resource is an attachment, suggested file name \'");
        stringBuilder.append(toWTFString(filename));
        stringBuilder.appendLiteral("\'\n");
        InjectedBundle::shared().outputText(stringBuilder.toString());
    }

    WKRetainPtr<WKStringRef> mimeType = adoptWK(WKURLResponseCopyMIMEType(response));
    return WKBundlePageCanShowMIMEType(page, mimeType.get()) ? WKBundlePagePolicyActionUse : WKBundlePagePolicyActionPassThrough;
}
