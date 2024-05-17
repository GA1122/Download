void InjectedBundlePage::didBeginEditing(WKStringRef notificationName)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;
    if (!InjectedBundle::shared().testRunner()->shouldDumpEditingCallbacks())
        return;

    StringBuilder stringBuilder;
    stringBuilder.appendLiteral("EDITING DELEGATE: webViewDidBeginEditing:");
    stringBuilder.append(toWTFString(notificationName));
    stringBuilder.append('\n');
    InjectedBundle::shared().outputText(stringBuilder.toString());
}
