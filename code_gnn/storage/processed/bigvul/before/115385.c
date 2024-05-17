void InjectedBundlePage::didFailLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t identifier, WKErrorRef error)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    if (!InjectedBundle::shared().testRunner()->shouldDumpResourceLoadCallbacks())
        return;

    StringBuilder stringBuilder;
    dumpResourceURL(identifier, stringBuilder);
    stringBuilder.appendLiteral(" - didFailLoadingWithError: ");

    dumpErrorDescriptionSuitableForTestResult(error, stringBuilder);
    stringBuilder.append('\n');
    InjectedBundle::shared().outputText(stringBuilder.toString());
}
