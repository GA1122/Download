void RenderLayerScrollableArea::updateResizerAreaSet()
{
    LocalFrame* frame = box().frame();
    if (!frame)
        return;
    FrameView* frameView = frame->view();
    if (!frameView)
        return;
    if (box().canResize())
        frameView->addResizerArea(box());
    else
        frameView->removeResizerArea(box());
}
