IntSize ScrollbarThemeWin::buttonSize(ScrollbarThemeClient* scrollbar)
{


    int thickness = scrollbarThickness(scrollbar->controlSize());

    const int kLayoutTestModeGirth = 17;
    int girth = isRunningLayoutTest() ? kLayoutTestModeGirth : thickness;

    if (scrollbar->orientation() == HorizontalScrollbar) {
        int width = scrollbar->width() < 2 * girth ? scrollbar->width() / 2 : girth;
        return IntSize(width, thickness);
    }

    int height = scrollbar->height() < 2 * girth ? scrollbar->height() / 2 : girth;
    return IntSize(thickness, height);
}
