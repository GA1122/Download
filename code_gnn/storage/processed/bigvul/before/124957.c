bool RenderBox::shrinkToAvoidFloats() const
{
    if ((isInline() && !isMarquee()) || !avoidsFloats() || isFloating())
        return false;

    return style()->width().isAuto();
}
