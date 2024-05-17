IntRect FrameView::scrollableAreaBoundingBox() const
{
    RenderPart* ownerRenderer = frame().ownerRenderer();
    if (!ownerRenderer)
        return frameRect();

    return ownerRenderer->absoluteContentQuad().enclosingBoundingBox();
}
