double Document::timerAlignmentInterval() const
{
    Page* p = page();
    if (!p)
        return ExecutionContext::timerAlignmentInterval();
    return p->timerAlignmentInterval();
}
