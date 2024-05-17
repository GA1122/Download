bool InjectedBundlePage::shouldEndEditing(WKBundleRangeHandleRef range)
{
    if (!InjectedBundle::shared().isTestRunning())
        return true;

    if (InjectedBundle::shared().testRunner()->shouldDumpEditingCallbacks()) {
        StringBuilder stringBuilder;
        stringBuilder.appendLiteral("EDITING DELEGATE: shouldEndEditingInDOMRange:");
        stringBuilder.append(rangeToStr(m_page, m_world.get(), range));
        stringBuilder.append('\n');
        InjectedBundle::shared().outputText(stringBuilder.toString());
    }
    return InjectedBundle::shared().testRunner()->shouldAllowEditing();
}
