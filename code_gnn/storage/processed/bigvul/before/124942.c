void RenderBox::scrollByRecursively(const IntSize& delta, ScrollOffsetClamping clamp)
{
    if (delta.isZero())
        return;

    bool restrictedByLineClamp = false;
    if (parent())
        restrictedByLineClamp = !parent()->style()->lineClamp().isNone();

    if (hasOverflowClip() && !restrictedByLineClamp) {
        IntSize newScrollOffset = layer()->scrollableArea()->adjustedScrollOffset() + delta;
        layer()->scrollableArea()->scrollToOffset(newScrollOffset, clamp);

        IntSize remainingScrollOffset = newScrollOffset - layer()->scrollableArea()->adjustedScrollOffset();
        if (!remainingScrollOffset.isZero() && parent()) {
            if (RenderBox* scrollableBox = enclosingScrollableBox())
                scrollableBox->scrollByRecursively(remainingScrollOffset, clamp);

            LocalFrame* frame = this->frame();
            if (frame && frame->page())
                frame->page()->autoscrollController().updateAutoscrollRenderer();
        }
    } else if (view()->frameView()) {
        view()->frameView()->scrollBy(delta);

    }
}
