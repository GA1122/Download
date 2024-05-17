bool RenderBox::computeBackgroundIsKnownToBeObscured()
{
    if (!hasBackground())
        return false;
    if (isTable() || isDocumentElement())
        return false;
    if (style()->boxShadow())
        return false;
    LayoutRect backgroundRect;
    if (!getBackgroundPaintedExtent(backgroundRect))
        return false;
    return foregroundIsKnownToBeOpaqueInRect(backgroundRect, backgroundObscurationTestMaxDepth);
}
