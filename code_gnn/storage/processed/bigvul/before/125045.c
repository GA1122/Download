static inline bool preferredMainAxisExtentDependsOnLayout(const Length& flexBasis, bool hasInfiniteLineLength)
{
    return flexBasis.isAuto() || (flexBasis.isFixed() && !flexBasis.value() && hasInfiniteLineLength);
}
