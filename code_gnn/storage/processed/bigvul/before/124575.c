static inline bool isSpaceForFirstLetter(UChar c)
{
    return isSpaceOrNewline(c) || c == noBreakSpace;
}
