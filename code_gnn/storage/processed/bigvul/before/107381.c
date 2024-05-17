Scrollbar::Scrollbar(ScrollableArea* scrollableArea, ScrollbarOrientation orientation, ScrollbarControlSize controlSize,
                     ScrollbarTheme* theme)
    : m_scrollableArea(scrollableArea)
    , m_orientation(orientation)
    , m_controlSize(controlSize)
    , m_theme(theme)
    , m_visibleSize(0)
    , m_totalSize(0)
    , m_currentPos(0)
    , m_dragOrigin(0)
    , m_lineStep(0)
    , m_pageStep(0)
    , m_pixelStep(1)
    , m_hoveredPart(NoPart)
    , m_pressedPart(NoPart)
    , m_pressedPos(0)
    , m_enabled(true)
    , m_scrollTimer(this, &Scrollbar::autoscrollTimerFired)
    , m_overlapsResizer(false)
    , m_suppressInvalidation(false)
{
    if (!m_theme)
        m_theme = ScrollbarTheme::nativeTheme();

    m_theme->registerScrollbar(this);

    int thickness = m_theme->scrollbarThickness(controlSize);
    Widget::setFrameRect(IntRect(0, 0, thickness, thickness));
}
