static LayoutUnit initialJustifyContentOffset(LayoutUnit availableFreeSpace, EJustifyContent justifyContent, unsigned numberOfChildren)
{
    if (justifyContent == JustifyFlexEnd)
        return availableFreeSpace;
    if (justifyContent == JustifyCenter)
        return availableFreeSpace / 2;
    if (justifyContent == JustifySpaceAround) {
        if (availableFreeSpace > 0 && numberOfChildren)
            return availableFreeSpace / (2 * numberOfChildren);
        else
            return availableFreeSpace / 2;
    }
    return 0;
}
