void RenderSVGImage::imageChanged(WrappedImagePtr, const IntRect*)
{
    if (SVGResources* resources = SVGResourcesCache::cachedResourcesForRenderObject(this))
        resources->removeClientFromCache(this);

    RenderSVGResource::markForLayoutAndParentResourceInvalidation(this, false);

    m_objectBoundingBox = FloatRect();
    updateImageViewport();

    invalidateBufferedForeground();

    repaint();
}
