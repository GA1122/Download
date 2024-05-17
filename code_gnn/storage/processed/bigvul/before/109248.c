void InspectorOverlay::paint(GraphicsContext& context)
{
    if (isEmpty())
        return;
    GraphicsContextStateSaver stateSaver(context);
    FrameView* view = overlayPage()->mainFrame()->view();
    ASSERT(!view->needsLayout());
    view->paint(&context, IntRect(0, 0, view->width(), view->height()));
}
