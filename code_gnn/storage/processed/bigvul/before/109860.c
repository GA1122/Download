void Document::updateLayout()
{
    ASSERT(isMainThread());

    FrameView* frameView = view();
    if (frameView && frameView->isInLayout()) {
        ASSERT_NOT_REACHED();
        return;
    }

    if (Element* oe = ownerElement())
        oe->document().updateLayout();

    updateStyleIfNeeded();

    if (frameView && renderer() && (frameView->layoutPending() || renderer()->needsLayout()))
        frameView->layout();

    if (frameView)
        frameView->partialLayout().reset();

    setNeedsFocusedElementCheck();
}
