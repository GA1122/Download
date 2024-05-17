Platform::IntRect WebPagePrivate::focusNodeRect()
{
    Frame* frame = focusedOrMainFrame();
    if (!frame)
        return Platform::IntRect();

    Document* doc = frame->document();
    FrameView* view = frame->view();
    if (!doc || !view || view->needsLayout())
        return Platform::IntRect();

    IntRect focusRect = rectForNode(doc->focusedNode());
    focusRect = adjustRectOffsetForFrameOffset(focusRect, doc->focusedNode());
    focusRect = mapToTransformed(focusRect);
    clipToTransformedContentsRect(focusRect);
    return focusRect;
}
