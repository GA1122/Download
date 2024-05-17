static bool columnFlexItemHasStretchAlignment(const RenderObject* flexitem)
{
    RenderObject* parent = flexitem->parent();
    ASSERT(parent->style()->isColumnFlexDirection());
    if (flexitem->style()->marginStart().isAuto() || flexitem->style()->marginEnd().isAuto())
        return false;
    return flexitem->style()->alignSelf() == ItemPositionStretch || (flexitem->style()->alignSelf() == ItemPositionAuto && parent->style()->alignItems() == ItemPositionStretch);
}
