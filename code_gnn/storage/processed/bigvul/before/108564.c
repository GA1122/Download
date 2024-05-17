void ScrollbarThemeWin::paintTrackPiece(GraphicsContext* gc, ScrollbarThemeClient* scrollbar, const IntRect& rect, ScrollbarPart partType)
{
    bool horz = scrollbar->orientation() == HorizontalScrollbar;

    int partId;
    if (partType == BackTrackPart)
        partId = horz ? SBP_UPPERTRACKHORZ : SBP_UPPERTRACKVERT;
    else
        partId = horz ? SBP_LOWERTRACKHORZ : SBP_LOWERTRACKVERT;

    IntRect alignRect = trackRect(scrollbar, false);

    WebKit::WebCanvas* canvas = gc->canvas();
    WebKit::Platform::current()->themeEngine()->paintScrollbarTrack(canvas, partId, getThemeState(scrollbar, partType), getClassicThemeState(scrollbar, partType), WebKit::WebRect(rect), WebKit::WebRect(alignRect));
}
