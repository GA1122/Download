bool RenderBlock::hitTestColumns(const HitTestRequest& request, HitTestResult& result, const HitTestLocation& locationInContainer, const LayoutPoint& accumulatedOffset, HitTestAction hitTestAction)
{
    if (!hasColumns())
        return false;

    for (ColumnRectIterator it(*this); it.hasMore(); it.advance()) {
        LayoutRect hitRect = locationInContainer.boundingBox();
        LayoutRect colRect = it.columnRect();
        colRect.moveBy(accumulatedOffset);
        if (locationInContainer.intersects(colRect)) {
            LayoutSize offset;
            it.adjust(offset);
            LayoutPoint finalLocation = accumulatedOffset + offset;
            if (!result.isRectBasedTest() || colRect.contains(hitRect))
                return hitTestContents(request, result, locationInContainer, finalLocation, hitTestAction) || (hitTestAction == HitTestFloat && hitTestFloats(request, result, locationInContainer, finalLocation));

            hitTestContents(request, result, locationInContainer, finalLocation, hitTestAction);
        }
    }

    return false;
}
