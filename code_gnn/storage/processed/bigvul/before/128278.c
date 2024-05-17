GraphicsLayer* FrameView::layerForScrolling() const
{
    RenderView* renderView = this->renderView();
    if (!renderView)
        return 0;
    return renderView->compositor()->scrollLayer();
}
