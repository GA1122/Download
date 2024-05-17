void InjectedBundlePage::didReceiveTitleForFrame(WKStringRef title, WKBundleFrameRef frame)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    StringBuilder stringBuilder;
    if (InjectedBundle::shared().testRunner()->shouldDumpFrameLoadCallbacks()) {
        dumpFrameDescriptionSuitableForTestResult(frame, stringBuilder);
        stringBuilder.appendLiteral(" - didReceiveTitle: ");
        stringBuilder.append(toWTFString(title));
        stringBuilder.append('\n');
    }

    if (InjectedBundle::shared().testRunner()->shouldDumpTitleChanges()) {
        stringBuilder.appendLiteral("TITLE CHANGED: '");
        stringBuilder.append(toWTFString(title));
        stringBuilder.appendLiteral("'\n");
    }

    InjectedBundle::shared().outputText(stringBuilder.toString());
}
