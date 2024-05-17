void FrameView::updateCounters()
{
    RenderView* view = renderView();
    if (!view->hasRenderCounters())
        return;

    for (RenderObject* renderer = view; renderer; renderer = renderer->nextInPreOrder()) {
        if (!renderer->isCounter())
            continue;

        toRenderCounter(renderer)->updateCounter();
    }
}
