static const char* findEndOfLine(const char* bytes, unsigned length)
{
    for (unsigned i = 0; i < length; i++) {
        if (bytes[i] == '\n')
            return bytes + i;
        if (bytes[i] == '\r') {
            if (i + 1 == length)
                break;

            return bytes + i;
        }
    }

    return 0;
}
