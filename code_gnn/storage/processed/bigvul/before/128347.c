void FrameView::setParent(Widget* widget)
{
    ScrollView::setParent(widget);
    updateScrollableAreaSet();
}
