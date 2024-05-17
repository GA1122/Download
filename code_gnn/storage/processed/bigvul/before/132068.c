void LayoutBlockFlow::getSelectionGapInfo(SelectionState state, bool& leftGap, bool& rightGap) const
{
    bool ltr = style()->isLeftToRightDirection();
    leftGap = (state == SelectionInside)
        || (state == SelectionEnd && ltr)
        || (state == SelectionStart && !ltr);
    rightGap = (state == SelectionInside)
        || (state == SelectionStart && ltr)
         || (state == SelectionEnd && !ltr);
 }
