void RenderBlockFlow::createOrDestroyMultiColumnFlowThreadIfNeeded()
{
    if (!document().regionBasedColumnsEnabled())
        return;

    bool needsFlowThread = style()->specifiesColumns();
    if (needsFlowThread != static_cast<bool>(multiColumnFlowThread())) {
        if (needsFlowThread) {
            RenderMultiColumnFlowThread* flowThread = RenderMultiColumnFlowThread::createAnonymous(document(), style());
            addChild(flowThread);
            flowThread->populate();
            RenderBlockFlowRareData& rareData = ensureRareData();
            ASSERT(!rareData.m_multiColumnFlowThread);
            rareData.m_multiColumnFlowThread = flowThread;
        } else {
            multiColumnFlowThread()->evacuateAndDestroy();
            ASSERT(!multiColumnFlowThread());
        }
    }
}
