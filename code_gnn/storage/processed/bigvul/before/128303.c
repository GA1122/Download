void FrameView::removeChild(Widget* widget)
{
    if (widget->isFrameView())
        removeScrollableArea(toFrameView(widget));

    ScrollView::removeChild(widget);
}
