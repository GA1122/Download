static bool isValidNameNonASCII(const UChar* characters, unsigned length)
{
    for (unsigned i = 0; i < length;) {
        bool first = i == 0;
        UChar32 c;
        U16_NEXT(characters, i, length, c);  
        if (first ? !isValidNameStart(c) : !isValidNamePart(c))
            return false;
    }

    return true;
}
