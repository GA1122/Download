static inline bool isValidNamePart(UChar32 c)
{
    if (isValidNameStart(c))
        return true;

    if (c == 0x00B7 || c == 0x0387)
        return true;

    if (c == '-' || c == '.')
        return true;

    const uint32_t otherNamePartMask = Mark_NonSpacing | Mark_Enclosing | Mark_SpacingCombining | Letter_Modifier | Number_DecimalDigit;
    if (!(Unicode::category(c) & otherNamePartMask))
        return false;

    if (c >= 0xF900 && c < 0xFFFE)
        return false;

    CharDecompositionType decompType = decompositionType(c);
    if (decompType == DecompositionFont || decompType == DecompositionCompat)
        return false;

    return true;
}
