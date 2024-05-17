static bool startsWithBlankLine(const char* bytes, unsigned length)
{
    return length > 0 && bytes[0] == '\n';
}
