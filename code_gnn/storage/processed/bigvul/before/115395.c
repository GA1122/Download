void InjectedBundlePage::didFinishLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t identifier)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    if (!InjectedBundle::shared().testRunner()->shouldDumpResourceLoadCallbacks())
        return;

    StringBuilder stringBuilder;
    dumpResourceURL(identifier, stringBuilder);
    stringBuilder.appendLiteral(" - didFinishLoading\n");
    InjectedBundle::shared().outputText(stringBuilder.toString());
}
