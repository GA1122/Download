bool InjectedBundlePage::shouldChangeSelectedRange(WKBundleRangeHandleRef fromRange, WKBundleRangeHandleRef toRange, WKAffinityType affinity, bool stillSelecting)
{
    if (!InjectedBundle::shared().isTestRunning())
        return true;

    static const char *affinitystring[] = {
        "NSSelectionAffinityUpstream",
        "NSSelectionAffinityDownstream"
    };
    static const char *boolstring[] = {
        "FALSE",
        "TRUE"
    };

    if (InjectedBundle::shared().testRunner()->shouldDumpEditingCallbacks()) {
        StringBuilder stringBuilder;
        stringBuilder.appendLiteral("EDITING DELEGATE: shouldChangeSelectedDOMRange:");
        stringBuilder.append(rangeToStr(m_page, m_world.get(), fromRange));
        stringBuilder.appendLiteral(" toDOMRange:");
        stringBuilder.append(rangeToStr(m_page, m_world.get(), toRange));
        stringBuilder.appendLiteral(" affinity:");
        stringBuilder.append(affinitystring[affinity]); 
        stringBuilder.appendLiteral(" stillSelecting:");
        stringBuilder.append(boolstring[stillSelecting]); 
        stringBuilder.append('\n');
        InjectedBundle::shared().outputText(stringBuilder.toString());
    }
    return InjectedBundle::shared().testRunner()->shouldAllowEditing();
}
