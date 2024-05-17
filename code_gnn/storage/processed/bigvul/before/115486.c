void InjectedBundlePage::willRunJavaScriptPrompt(WKStringRef message, WKStringRef defaultValue, WKBundleFrameRef)
{
    StringBuilder stringBuilder;
    stringBuilder.appendLiteral("PROMPT: ");
    stringBuilder.append(toWTFString(message));
    stringBuilder.appendLiteral(", default text: ");
    stringBuilder.append(toWTFString(defaultValue));
    stringBuilder.append('\n');
    InjectedBundle::shared().outputText(stringBuilder.toString());
}
