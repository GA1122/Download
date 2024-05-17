bool InjectedBundlePage::shouldCacheResponse(WKBundlePageRef, WKBundleFrameRef, uint64_t identifier)
{
    if (!InjectedBundle::shared().isTestRunning())
        return true;

    if (!InjectedBundle::shared().testRunner()->shouldDumpWillCacheResponse())
        return true;

    StringBuilder stringBuilder;
    stringBuilder.appendNumber(identifier);
    stringBuilder.appendLiteral(" - willCacheResponse: called\n");
    InjectedBundle::shared().outputText(stringBuilder.toString());

    return true;
}
