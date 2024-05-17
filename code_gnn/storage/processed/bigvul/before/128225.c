IntPoint FrameView::clampOffsetAtScale(const IntPoint& offset, float scale) const
{
    IntPoint maxScrollExtent(contentsSize().width() - scrollOrigin().x(), contentsSize().height() - scrollOrigin().y());
    FloatSize scaledSize = unscaledVisibleContentSize();
    if (scale)
        scaledSize.scale(1 / scale);

    IntPoint clampedOffset = offset;
    clampedOffset = clampedOffset.shrunkTo(maxScrollExtent - expandedIntSize(scaledSize));
    clampedOffset = clampedOffset.expandedTo(-scrollOrigin());

    return clampedOffset;
}
