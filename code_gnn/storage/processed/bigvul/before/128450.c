bool RenderLayerScrollableArea::shouldSuspendScrollAnimations() const
{
    RenderView* view = box().view();
    if (!view)
        return true;
    return view->frameView()->shouldSuspendScrollAnimations();
}
