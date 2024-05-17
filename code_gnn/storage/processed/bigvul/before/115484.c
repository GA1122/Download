void InjectedBundlePage::willRunJavaScriptConfirm(WKStringRef message, WKBundleFrameRef)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    StringBuilder stringBuilder;
    stringBuilder.appendLiteral("CONFIRM: ");
    stringBuilder.append(toWTFString(message));
    stringBuilder.append('\n');
    InjectedBundle::shared().outputText(stringBuilder.toString());
}
