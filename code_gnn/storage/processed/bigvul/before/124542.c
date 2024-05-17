void RenderBlock::dirtyForLayoutFromPercentageHeightDescendants(SubtreeLayoutScope& layoutScope)
{
    if (!gPercentHeightDescendantsMap)
        return;

    TrackedRendererListHashSet* descendants = gPercentHeightDescendantsMap->get(this);
    if (!descendants)
        return;

    TrackedRendererListHashSet::iterator end = descendants->end();
    for (TrackedRendererListHashSet::iterator it = descendants->begin(); it != end; ++it) {
        RenderBox* box = *it;
        while (box != this) {
            if (box->normalChildNeedsLayout())
                break;
            layoutScope.setChildNeedsLayout(box);
            box = box->containingBlock();
            ASSERT(box);
            if (!box)
                break;
        }
    }
}
