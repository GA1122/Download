static bool shouldFlipBeforeAfterMargins(const RenderStyle* containingBlockStyle, const RenderStyle* childStyle)
{
    ASSERT(containingBlockStyle->isHorizontalWritingMode() != childStyle->isHorizontalWritingMode());
    WritingMode childWritingMode = childStyle->writingMode();
    bool shouldFlip = false;
    switch (containingBlockStyle->writingMode()) {
    case TopToBottomWritingMode:
        shouldFlip = (childWritingMode == RightToLeftWritingMode);
        break;
    case BottomToTopWritingMode:
        shouldFlip = (childWritingMode == RightToLeftWritingMode);
        break;
    case RightToLeftWritingMode:
        shouldFlip = (childWritingMode == BottomToTopWritingMode);
        break;
    case LeftToRightWritingMode:
        shouldFlip = (childWritingMode == BottomToTopWritingMode);
        break;
    }

    if (!containingBlockStyle->isLeftToRightDirection())
        shouldFlip = !shouldFlip;

    return shouldFlip;
}
