Document::~Document()
{
    ASSERT(!layoutView());
    ASSERT(!parentTreeScope());
    ASSERT(!m_axObjectCache);
#if !ENABLE(OILPAN)
    ASSERT(m_ranges.isEmpty());
    ASSERT(!hasGuardRefCount());
    ASSERT(!m_importsController);
    ASSERT(m_visibilityObservers.isEmpty());

    if (m_templateDocument)
        m_templateDocument->m_templateDocumentHost = nullptr;  

    m_scriptRunner.clear();

    removeAllEventListenersRecursively();

    ASSERT(!m_parser || m_parser->refCount() == 1);
    detachParser();

    if (m_styleSheetList)
        m_styleSheetList->detachFromDocument();

    m_timeline->detachFromDocument();

    m_styleEngine->detachFromDocument();

    if (m_elemSheet)
        m_elemSheet->clearOwnerNode();

    if (hasRareData())
        clearRareData();

    ASSERT(m_listsInvalidatedAtDocument.isEmpty());

    for (unsigned i = 0; i < WTF_ARRAY_LENGTH(m_nodeListCounts); ++i)
        ASSERT(!m_nodeListCounts[i]);

    liveDocumentSet().remove(this);
#endif

    InstanceCounters::decrementCounter(InstanceCounters::DocumentCounter);
}
