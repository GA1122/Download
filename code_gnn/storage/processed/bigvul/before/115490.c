void InjectedBundlePage::willSetStatusbarText(WKStringRef statusbarText)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    if (!InjectedBundle::shared().testRunner()->shouldDumpStatusCallbacks())
        return;

    StringBuilder stringBuilder;
    stringBuilder.appendLiteral("UI DELEGATE STATUS CALLBACK: setStatusText:");
    stringBuilder.append(toWTFString(statusbarText));
    stringBuilder.append('\n');
    InjectedBundle::shared().outputText(stringBuilder.toString());
}
