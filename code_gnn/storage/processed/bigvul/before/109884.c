Document::~Document()
{
    ASSERT(!renderer());
    ASSERT(m_ranges.isEmpty());
    ASSERT(!m_parentTreeScope);
    ASSERT(!hasGuardRefCount());

    if (m_templateDocument)
        m_templateDocument->setTemplateDocumentHost(0);  

    if (Document* ownerDocument = this->ownerDocument())
        ownerDocument->didRemoveEventTargetNode(this);

    m_scriptRunner.clear();

    removeAllEventListeners();

    ASSERT(!m_parser || m_parser->refCount() == 1);
    detachParser();

    if (this == topDocument())
        clearAXObjectCache();

    setDecoder(PassRefPtr<TextResourceDecoder>());

    if (m_styleSheetList)
        m_styleSheetList->detachFromDocument();

    if (m_import) {
        m_import->wasDetachedFromDocument();
        m_import = 0;
    }

    m_styleEngine.clear();

    if (m_elemSheet)
        m_elemSheet->clearOwnerNode();

    clearStyleResolver();  

    if (m_fetcher->document() == this)
        m_fetcher->setDocument(0);
    m_fetcher.clear();

    if (hasRareData())
        clearRareData();

    ASSERT(!m_listsInvalidatedAtDocument.size());

    for (unsigned i = 0; i < WTF_ARRAY_LENGTH(m_nodeListCounts); i++)
        ASSERT(!m_nodeListCounts[i]);

    clearDocumentScope();
    setClient(0);

    InspectorCounters::decrementCounter(InspectorCounters::DocumentCounter);
}
