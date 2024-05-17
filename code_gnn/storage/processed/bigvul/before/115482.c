void InjectedBundlePage::willRunJavaScriptAlert(WKStringRef message, WKBundleFrameRef)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    StringBuilder stringBuilder;
    stringBuilder.appendLiteral("ALERT: ");
    stringBuilder.append(toWTFString(message));
    stringBuilder.append('\n');
    InjectedBundle::shared().outputText(stringBuilder.toString());
}
