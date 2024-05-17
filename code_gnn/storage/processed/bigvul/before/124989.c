int RenderFlexibleBox::baselinePosition(FontBaseline, bool, LineDirectionMode direction, LinePositionMode mode) const
{
    ASSERT(mode == PositionOnContainingLine);
    int baseline = firstLineBoxBaseline();
    if (baseline == -1)
        baseline = synthesizedBaselineFromContentBox(this, direction);

    return beforeMarginInLineDirection(direction) + baseline;
}
