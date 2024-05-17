bool RenderBox::canResize() const
{
    return (hasOverflowClip() || isRenderIFrame()) && style()->resize() != RESIZE_NONE;
}
