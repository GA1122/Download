int RenderFlexibleBox::inlineBlockBaseline(LineDirectionMode direction) const
{
    int baseline = firstLineBoxBaseline();
    if (baseline != -1)
        return baseline;

    int marginAscent = direction == HorizontalLine ? marginTop() : marginRight();
    return synthesizedBaselineFromContentBox(this, direction) + marginAscent;
}
