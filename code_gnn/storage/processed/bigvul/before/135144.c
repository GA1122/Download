void Document::attach(const AttachContext& context)
{
    ASSERT(m_lifecycle.state() == DocumentLifecycle::Inactive);
    ASSERT(!m_axObjectCache || this != &axObjectCacheOwner());

    m_layoutView = new LayoutView(this);
    setLayoutObject(m_layoutView);

    m_layoutView->setIsInWindow(true);
    m_layoutView->setStyle(StyleResolver::styleForDocument(*this));
    m_layoutView->compositor()->setNeedsCompositingUpdate(CompositingUpdateAfterCompositingInputChange);

    ContainerNode::attach(context);

    if (TextAutosizer* autosizer = textAutosizer())
        autosizer->updatePageInfo();

    m_lifecycle.advanceTo(DocumentLifecycle::StyleClean);
}
