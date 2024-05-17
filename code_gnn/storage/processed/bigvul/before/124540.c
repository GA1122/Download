void RenderBlock::deleteLineBoxTree()
{
    m_lineBoxes.deleteLineBoxTree();

    if (AXObjectCache* cache = document().existingAXObjectCache())
        cache->recomputeIsIgnored(this);
}
