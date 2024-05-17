void InjectedBundlePage::didChangeSelection(WKStringRef notificationName)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;
    if (!InjectedBundle::shared().testRunner()->shouldDumpEditingCallbacks())
        return;

    StringBuilder stringBuilder;
    stringBuilder.appendLiteral("EDITING DELEGATE: webViewDidChangeSelection:");
    stringBuilder.append(toWTFString(notificationName));
    stringBuilder.append('\n');
    InjectedBundle::shared().outputText(stringBuilder.toString());
}
