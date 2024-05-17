static LayoutUnit alignContentSpaceBetweenChildren(LayoutUnit availableFreeSpace, EAlignContent alignContent, unsigned numberOfLines)
{
    if (availableFreeSpace > 0 && numberOfLines > 1) {
        if (alignContent == AlignContentSpaceBetween)
            return availableFreeSpace / (numberOfLines - 1);
        if (alignContent == AlignContentSpaceAround || alignContent == AlignContentStretch)
            return availableFreeSpace / numberOfLines;
    }
    return 0;
}
