bool RenderBlock::hasMarginBeforeQuirk(const RenderBox* child) const
{
    if (!child->isWritingModeRoot())
        return child->isRenderBlock() ? toRenderBlock(child)->hasMarginBeforeQuirk() : child->style()->hasMarginBeforeQuirk();

    if (child->isHorizontalWritingMode() == isHorizontalWritingMode())
        return child->isRenderBlock() ? toRenderBlock(child)->hasMarginAfterQuirk() : child->style()->hasMarginAfterQuirk();

    return false;
}