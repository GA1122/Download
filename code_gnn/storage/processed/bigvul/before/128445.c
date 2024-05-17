bool RenderLayerScrollableArea::scrollbarsCanBeActive() const
{
    RenderView* view = box().view();
    if (!view)
        return false;
    return view->frameView()->scrollbarsCanBeActive();
}
