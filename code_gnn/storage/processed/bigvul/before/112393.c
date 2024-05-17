void Document::attach()
{
    ASSERT(!attached());
    ASSERT(!m_inPageCache);
    ASSERT(!m_axObjectCache || this != topDocument());

    if (!m_renderArena)
        m_renderArena = adoptPtr(new RenderArena);
    
    setRenderer(new (m_renderArena.get()) RenderView(this));
#if USE(ACCELERATED_COMPOSITING)
    renderView()->setIsInWindow(true);
#endif

    recalcStyle(Force);

    RenderObject* render = renderer();
    setRenderer(0);

    ContainerNode::attach();

    setRenderer(render);
}
