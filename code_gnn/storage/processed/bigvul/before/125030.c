bool RenderFlexibleBox::isMultiline() const
{
    return style()->flexWrap() != FlexNoWrap;
}
