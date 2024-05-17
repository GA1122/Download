bool CSPSourceList::parseNonce(const UChar* begin, const UChar* end, String& nonce)
{
    size_t nonceLength = end - begin;
    const char* prefix = "'nonce-";

    if (nonceLength <= strlen(prefix) || !equalIgnoringCase(prefix, begin, strlen(prefix)))
        return true;

    const UChar* position = begin + strlen(prefix);
    const UChar* nonceBegin = position;

    ASSERT(position < end);
    skipWhile<UChar, isNonceCharacter>(position, end);
    ASSERT(nonceBegin <= position);

    if (position + 1 != end || *position != '\'' || position == nonceBegin)
        return false;

    nonce = String(nonceBegin, position - nonceBegin);
    return true;
}
