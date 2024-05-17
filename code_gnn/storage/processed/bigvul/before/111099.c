bool WebPagePrivate::compositorDrawsRootLayer() const
{
#if USE(ACCELERATED_COMPOSITING)
    if (Platform::userInterfaceThreadMessageClient()->isCurrentThread())
        return m_compositor && m_compositor->drawsRootLayer();

    RenderView* renderView = m_mainFrame->contentRenderer();
    if (!renderView || !renderView->layer() || !renderView->layer()->backing())
        return false;

    return !renderView->layer()->backing()->paintingGoesToWindow();
#else
    return false;
#endif
}
