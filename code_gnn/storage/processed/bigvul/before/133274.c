void RenderSVGImage::layout()
{
    ASSERT(needsLayout());

    LayoutRectRecorder recorder(*this);
    LayoutRepainter repainter(*this, SVGRenderSupport::checkForSVGRepaintDuringLayout(this) && selfNeedsLayout());
    updateImageViewport();

    bool transformOrBoundariesUpdate = m_needsTransformUpdate || m_needsBoundariesUpdate;
    if (m_needsTransformUpdate) {
        m_localTransform = toSVGImageElement(element())->animatedLocalTransform();
        m_needsTransformUpdate = false;
    }

    if (m_needsBoundariesUpdate) {
        m_repaintBoundingBox = m_objectBoundingBox;
        SVGRenderSupport::intersectRepaintRectWithResources(this, m_repaintBoundingBox);

        m_needsBoundariesUpdate = false;
    }

    if (everHadLayout() && selfNeedsLayout())
        SVGResourcesCache::clientLayoutChanged(this);

    if (transformOrBoundariesUpdate)
        RenderSVGModelObject::setNeedsBoundariesUpdate();

    repainter.repaintAfterLayout();
    clearNeedsLayout();
}
