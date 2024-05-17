bool RenderBox::foregroundIsKnownToBeOpaqueInRect(const LayoutRect& localRect, unsigned maxDepthToTest) const
{
    if (!maxDepthToTest)
        return false;
    for (RenderObject* child = firstChild(); child; child = child->nextSibling()) {
        if (!child->isBox())
            continue;
        RenderBox* childBox = toRenderBox(child);
        if (!isCandidateForOpaquenessTest(childBox))
            continue;
        LayoutPoint childLocation = childBox->location();
        if (childBox->isRelPositioned())
            childLocation.move(childBox->relativePositionOffset());
        LayoutRect childLocalRect = localRect;
        childLocalRect.moveBy(-childLocation);
        if (childLocalRect.y() < 0 || childLocalRect.x() < 0) {
            if (childBox->style()->position() == StaticPosition)
                return false;
            continue;
        }
        if (childLocalRect.maxY() > childBox->height() || childLocalRect.maxX() > childBox->width())
            continue;
        if (childBox->backgroundIsKnownToBeOpaqueInRect(childLocalRect))
            return true;
        if (childBox->foregroundIsKnownToBeOpaqueInRect(childLocalRect, maxDepthToTest - 1))
            return true;
    }
    return false;
}
