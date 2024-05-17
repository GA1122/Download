bool CSPSourceList::parsePort(const UChar* begin, const UChar* end, int& port, CSPSource::WildcardDisposition& portWildcard)
{
    ASSERT(begin <= end);
    ASSERT(!port);
    ASSERT(portWildcard == CSPSource::NoWildcard);

    if (!skipExactly<UChar>(begin, end, ':'))
        ASSERT_NOT_REACHED();

    if (begin == end)
        return false;

    if (end - begin == 1 && *begin == '*') {
        port = 0;
        portWildcard = CSPSource::HasWildcard;
        return true;
    }

    const UChar* position = begin;
    skipWhile<UChar, isASCIIDigit>(position, end);

    if (position != end)
        return false;

    bool ok;
    port = charactersToIntStrict(begin, end - begin, &ok);
    return ok;
}
