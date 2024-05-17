inline void FrameView::forceLayoutParentViewIfNeeded()
{
    RenderPart* ownerRenderer = m_frame->ownerRenderer();
    if (!ownerRenderer || !ownerRenderer->frame())
        return;

    RenderBox* contentBox = embeddedContentBox();
    if (!contentBox)
        return;

    RenderSVGRoot* svgRoot = toRenderSVGRoot(contentBox);
    if (svgRoot->everHadLayout() && !svgRoot->needsLayout())
        return;

    RefPtr<FrameView> frameView = ownerRenderer->frame()->view();

    ownerRenderer->setNeedsLayoutAndPrefWidthsRecalcAndFullPaintInvalidation();

    ASSERT(frameView);
    frameView->layout();
}
