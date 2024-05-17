void LayoutBlockFlow::createOrDestroyMultiColumnFlowThreadIfNeeded(const ComputedStyle* oldStyle)
{
    FlowThreadType type = flowThreadType(styleRef());

    if (multiColumnFlowThread()) {
        ASSERT(oldStyle);
        if (type != flowThreadType(*oldStyle)) {
            multiColumnFlowThread()->evacuateAndDestroy();
            ASSERT(!multiColumnFlowThread());
        }
    }

    if (type == NoFlowThread || multiColumnFlowThread())
        return;

    if (isRuby())
        return;

    if (isFieldset())
        return;

    if (isFileUploadControl() || isTextControl() || isListBox())
        return;

    LayoutMultiColumnFlowThread* flowThread = createMultiColumnFlowThread(type);
    addChild(flowThread);

    ASSERT(flowThread->parent() == this);

    flowThread->populate();
    LayoutBlockFlowRareData& rareData = ensureRareData();
    ASSERT(!rareData.m_multiColumnFlowThread);
    rareData.m_multiColumnFlowThread = flowThread;
}
