void FrameView::removeFromAXObjectCache()
{
    if (AXObjectCache* cache = axObjectCache())
        cache->remove(this);
}
