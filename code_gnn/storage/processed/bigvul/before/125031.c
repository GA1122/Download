static LayoutUnit justifyContentSpaceBetweenChildren(LayoutUnit availableFreeSpace, EJustifyContent justifyContent, unsigned numberOfChildren)
{
    if (availableFreeSpace > 0 && numberOfChildren > 1) {
        if (justifyContent == JustifySpaceBetween)
            return availableFreeSpace / (numberOfChildren - 1);
        if (justifyContent == JustifySpaceAround)
            return availableFreeSpace / numberOfChildren;
    }
    return 0;
}
