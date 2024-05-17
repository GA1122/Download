RootInlineBox* RenderBlockFlow::createAndAppendRootInlineBox()
{
    RootInlineBox* rootBox = createRootInlineBox();
    m_lineBoxes.appendLineBox(rootBox);

    if (UNLIKELY(AXObjectCache::accessibilityEnabled()) && m_lineBoxes.firstLineBox() == rootBox) {
        if (AXObjectCache* cache = document().existingAXObjectCache())
            cache->recomputeIsIgnored(this);
    }

    return rootBox;
}
