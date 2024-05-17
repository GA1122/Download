void RenderBox::paintBoxDecorationsWithRect(PaintInfo& paintInfo, const LayoutPoint& paintOffset, const LayoutRect& paintRect)
{
    BackgroundBleedAvoidance bleedAvoidance = determineBackgroundBleedAvoidance(paintInfo.context);

    if (!boxShadowShouldBeAppliedToBackground(bleedAvoidance))
        paintBoxShadow(paintInfo, paintRect, style(), Normal);

    GraphicsContextStateSaver stateSaver(*paintInfo.context, false);
    if (bleedAvoidance == BackgroundBleedClipBackground) {
        stateSaver.save();
        RoundedRect border = style()->getRoundedBorderFor(paintRect);
        paintInfo.context->clipRoundedRect(border);
    }

    paintBackgroundWithBorderAndBoxShadow(paintInfo, paintRect, bleedAvoidance);
}
