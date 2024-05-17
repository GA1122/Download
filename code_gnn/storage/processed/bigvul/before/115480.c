void InjectedBundlePage::willPerformClientRedirectForFrame(WKBundleFrameRef frame, WKURLRef url, double delay, double date)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    if (!InjectedBundle::shared().testRunner()->shouldDumpFrameLoadCallbacks())
        return;

    StringBuilder stringBuilder;
    dumpFrameDescriptionSuitableForTestResult(frame, stringBuilder);
    stringBuilder.appendLiteral(" - willPerformClientRedirectToURL: ");
    stringBuilder.append(pathSuitableForTestResult(url));
    stringBuilder.appendLiteral(" \n");
    InjectedBundle::shared().outputText(stringBuilder.toString());
}
