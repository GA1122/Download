bool RenderBox::usesCompositedScrolling() const
{
    return hasOverflowClip() && hasLayer() && layer()->scrollableArea()->usesCompositedScrolling();
}
