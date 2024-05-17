void Document::scheduleStyleRecalc()
{
    if (shouldDisplaySeamlesslyWithParent()) {
        ownerElement()->setNeedsStyleRecalc();
        ownerElement()->document()->scheduleStyleRecalc();
        return;
    }

    if (m_styleRecalcTimer.isActive() || inPageCache())
        return;

    ASSERT(childNeedsStyleRecalc() || m_pendingStyleRecalcShouldForce);

    if (!documentsThatNeedStyleRecalc)
        documentsThatNeedStyleRecalc = new HashSet<Document*>;
    documentsThatNeedStyleRecalc->add(this);
    
    invalidateAccessKeyMap();
    
    m_styleRecalcTimer.startOneShot(0);

    InspectorInstrumentation::didScheduleStyleRecalculation(this);
}
