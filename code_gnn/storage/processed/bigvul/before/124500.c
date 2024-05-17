int RenderBlock::beforeMarginInLineDirection(LineDirectionMode direction) const
{
    return direction == HorizontalLine ? marginTop() : marginRight();
}
