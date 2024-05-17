static bool isSourceListNone(const UChar* begin, const UChar* end)
{
    skipWhile<UChar, isASCIISpace>(begin, end);

    const UChar* position = begin;
    skipWhile<UChar, isSourceCharacter>(position, end);
    if (!equalIgnoringCase("'none'", begin, position - begin))
        return false;

    skipWhile<UChar, isASCIISpace>(position, end);
    if (position != end)
        return false;

    return true;
}
