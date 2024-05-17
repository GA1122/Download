void InjectedBundlePage::didEndEditing(WKStringRef notificationName)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;
    if (!InjectedBundle::shared().testRunner()->shouldDumpEditingCallbacks())
        return;

    StringBuilder stringBuilder;
    stringBuilder.appendLiteral("EDITING DELEGATE: webViewDidEndEditing:");
    stringBuilder.append(toWTFString(notificationName));
    stringBuilder.append('\n');
    InjectedBundle::shared().outputText(stringBuilder.toString());
}
