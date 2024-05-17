static bool shouldCoalesce(const IntRect& rect, const Vector<IntRect>& rects)
{
    const unsigned int cRectThreshold = 10;
    const float cWastedSpaceThreshold = 0.75f;
    bool useUnionedRect = (rects.size() <= 1) || (rects.size() > cRectThreshold);
    if (useUnionedRect)
        return true;
    float unionPixels = (rect.width() * rect.height());
    float singlePixels = 0;
    for (size_t i = 0; i < rects.size(); ++i)
        singlePixels += rects[i].width() * rects[i].height();
    float wastedSpace = 1 - (singlePixels / unionPixels);
    if (wastedSpace <= cWastedSpaceThreshold)
        useUnionedRect = true;
    return useUnionedRect;
}
