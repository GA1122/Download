LayoutUnit RenderBlockFlow::adjustLogicalLeftOffsetForLine(LayoutUnit offsetFromFloats, bool applyTextIndent) const
{
    LayoutUnit left = offsetFromFloats;

    if (applyTextIndent && style()->isLeftToRightDirection())
        left += textIndentOffset();

    return left;
}
