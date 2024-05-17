static void dumpLoadEvent(WKBundleFrameRef frame, const char* eventName)
{
    StringBuilder stringBuilder;
    dumpFrameDescriptionSuitableForTestResult(frame, stringBuilder);
    stringBuilder.appendLiteral(" - ");
    stringBuilder.append(eventName);
    stringBuilder.append('\n');
    InjectedBundle::shared().outputText(stringBuilder.toString());
}
