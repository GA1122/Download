PassRefPtr<Scrollbar> FrameView::createScrollbar(ScrollbarOrientation orientation)
{
    Element* customScrollbarElement = 0;
    LocalFrame* customScrollbarFrame = 0;
    if (shouldUseCustomScrollbars(customScrollbarElement, customScrollbarFrame))
        return RenderScrollbar::createCustomScrollbar(this, orientation, customScrollbarElement, customScrollbarFrame);

    return ScrollView::createScrollbar(orientation);
}
