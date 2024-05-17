void Document::childrenChanged(const ChildrenChange& change)
{
    ContainerNode::childrenChanged(change);
    m_documentElement = ElementTraversal::firstWithin(*this);

    if (m_documentElement && !isHTMLDocument())
        beginLifecycleUpdatesIfRenderingReady();
}
