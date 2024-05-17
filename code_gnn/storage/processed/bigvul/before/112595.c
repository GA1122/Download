double Document::timerAlignmentInterval() const
{
    Page* p = page();
    if (!p)
        return ScriptExecutionContext::timerAlignmentInterval();
    return p->settings()->domTimerAlignmentInterval();
}
