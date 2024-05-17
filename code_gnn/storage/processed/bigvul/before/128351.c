void FrameView::setTransparent(bool isTransparent)
{
    m_isTransparent = isTransparent;
    DisableCompositingQueryAsserts disabler;
    if (renderView() && renderView()->layer()->hasCompositedLayerMapping())
        renderView()->layer()->compositedLayerMapping()->updateContentsOpaque();
}
