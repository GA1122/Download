LayoutSize RenderBox::cachedSizeForOverflowClip() const
{
    ASSERT(hasOverflowClip());
    ASSERT(hasLayer());
    return layer()->size();
}
