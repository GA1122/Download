bool InjectedBundlePage::shouldDeleteRange(WKBundleRangeHandleRef range)
{
    if (!InjectedBundle::shared().isTestRunning())
        return true;

    if (InjectedBundle::shared().testRunner()->shouldDumpEditingCallbacks()) {
        StringBuilder stringBuilder;
        stringBuilder.appendLiteral("EDITING DELEGATE: shouldDeleteDOMRange:");
        stringBuilder.append(rangeToStr(m_page, m_world.get(), range));
        stringBuilder.append('\n');
        InjectedBundle::shared().outputText(stringBuilder.toString());
    }
    return InjectedBundle::shared().testRunner()->shouldAllowEditing();
}
