double Document::minimumTimerInterval() const
{
    Page* p = page();
    if (!p)
        return ScriptExecutionContext::minimumTimerInterval();
    return p->settings()->minDOMTimerInterval();
}
