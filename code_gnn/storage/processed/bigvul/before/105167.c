ShadowRoot::ShadowRoot(Document* document)
    : DocumentFragment(document, CreateShadowRoot)
    , TreeScope(this)
    , m_prev(0)
    , m_next(0)
    , m_applyAuthorStyles(false)
    , m_insertionPointAssignedTo(0)
{
    ASSERT(document);
    
    setParentTreeScope(document);
    ensureRareData()->setTreeScope(this);
}
