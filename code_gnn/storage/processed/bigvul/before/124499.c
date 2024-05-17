int RenderBlock::baselinePosition(FontBaseline baselineType, bool firstLine, LineDirectionMode direction, LinePositionMode linePositionMode) const
{
    if (isInline() && linePositionMode == PositionOnContainingLine) {
        if (style()->hasAppearance() && !RenderTheme::theme().isControlContainer(style()->appearance()))
            return RenderTheme::theme().baselinePosition(this);

        bool ignoreBaseline = (layer() && layer()->scrollableArea() && (isMarquee() || (direction == HorizontalLine ? (layer()->scrollableArea()->verticalScrollbar() || layer()->scrollableArea()->scrollYOffset())
            : (layer()->scrollableArea()->horizontalScrollbar() || layer()->scrollableArea()->scrollXOffset())))) || (isWritingModeRoot() && !isRubyRun());

        int baselinePos = ignoreBaseline ? -1 : inlineBlockBaseline(direction);

        if (isDeprecatedFlexibleBox()) {
            LayoutUnit bottomOfContent = direction == HorizontalLine ? borderTop() + paddingTop() + contentHeight() : borderRight() + paddingRight() + contentWidth();
            if (baselinePos > bottomOfContent)
                baselinePos = -1;
        }
        if (baselinePos != -1)
            return beforeMarginInLineDirection(direction) + baselinePos;

        return RenderBox::baselinePosition(baselineType, firstLine, direction, linePositionMode);
    }

    ASSERT(linePositionMode == PositionOfInteriorLineBoxes);

    const FontMetrics& fontMetrics = style(firstLine)->fontMetrics();
    return fontMetrics.ascent(baselineType) + (lineHeight(firstLine, direction, linePositionMode) - fontMetrics.height()) / 2;
}
