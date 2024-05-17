IntSize Document::viewportSize() const
{
    if (!view())
        return IntSize();
    return view()->visibleContentRect(ScrollableArea::IncludeScrollbars).size();
}
