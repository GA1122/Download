void Document::attach(const AttachContext& context)
{
    DCHECK_EQ(m_lifecycle.state(), DocumentLifecycle::Inactive);
    DCHECK(!m_axObjectCache || this != &axObjectCacheOwner());

    m_layoutView = new LayoutView(this);
    setLayoutObject(m_layoutView);

    m_layoutView->setIsInWindow(true);
    m_layoutView->setStyle(StyleResolver::styleForDocument(*this));
    m_layoutView->compositor()->setNeedsCompositingUpdate(CompositingUpdateAfterCompositingInputChange);

    ContainerNode::attach(context);

    if (TextAutosizer* autosizer = textAutosizer())
        autosizer->updatePageInfo();

    m_frame->selection().documentAttached(this);
    m_lifecycle.advanceTo(DocumentLifecycle::StyleClean);
}
