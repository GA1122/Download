void InjectedBundlePage::didChange(WKStringRef notificationName)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;
    if (!InjectedBundle::shared().testRunner()->shouldDumpEditingCallbacks())
        return;

    StringBuilder stringBuilder;
    stringBuilder.appendLiteral("EDITING DELEGATE: webViewDidChange:");
    stringBuilder.append(toWTFString(notificationName));
    stringBuilder.append('\n');
    InjectedBundle::shared().outputText(stringBuilder.toString());
}
