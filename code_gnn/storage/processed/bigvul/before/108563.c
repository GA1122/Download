void ScrollbarThemeWin::paintThumb(GraphicsContext* gc, ScrollbarThemeClient* scrollbar, const IntRect& rect)
{
    bool horz = scrollbar->orientation() == HorizontalScrollbar;

    WebKit::WebCanvas* canvas = gc->canvas();
    WebKit::Platform::current()->themeEngine()->paintScrollbarThumb(canvas, horz ? SBP_THUMBBTNHORZ : SBP_THUMBBTNVERT, getThemeState(scrollbar, ThumbPart), getClassicThemeState(scrollbar, ThumbPart), WebKit::WebRect(rect));

    WebKit::Platform::current()->themeEngine()->paintScrollbarThumb(canvas, horz ? SBP_GRIPPERHORZ : SBP_GRIPPERVERT, getThemeState(scrollbar, ThumbPart), getClassicThemeState(scrollbar, ThumbPart), WebKit::WebRect(rect));
}
