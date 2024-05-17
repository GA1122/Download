static LayoutUnit initialAlignContentOffset(LayoutUnit availableFreeSpace, EAlignContent alignContent, unsigned numberOfLines)
{
    if (alignContent == AlignContentFlexEnd)
        return availableFreeSpace;
    if (alignContent == AlignContentCenter)
        return availableFreeSpace / 2;
    if (alignContent == AlignContentSpaceAround) {
        if (availableFreeSpace > 0 && numberOfLines)
            return availableFreeSpace / (2 * numberOfLines);
        if (availableFreeSpace < 0)
            return availableFreeSpace / 2;
    }
    return 0;
}
