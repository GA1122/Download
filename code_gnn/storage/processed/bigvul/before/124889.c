void RenderBox::layout()
{
    ASSERT(needsLayout());

    RenderObject* child = firstChild();
    if (!child) {
        clearNeedsLayout();
        return;
    }

    LayoutStateMaintainer statePusher(*this, locationOffset());
    while (child) {
        child->layoutIfNeeded();
        ASSERT(!child->needsLayout());
        child = child->nextSibling();
    }
    invalidateBackgroundObscurationStatus();
    clearNeedsLayout();
}
