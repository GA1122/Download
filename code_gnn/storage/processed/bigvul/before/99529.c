static int locationAfterFirstBlankLine(const char* bytes, unsigned length)
{
    for (unsigned i = 0; i < length - 4; i++) {
        if (bytes[i] == '\n' && bytes[i + 1] == '\n')
            return i + 2;
        
        if (bytes[i] == '\r' && bytes[i + 1] == '\n') {
            i += 2;
            if (i == 2)
                return i;

            if (bytes[i] == '\n') {
                return i + 1;
            }

            if (bytes[i] == '\r' && bytes[i + 1] == '\n') {
                return i + 2;
            }
        }
    }

    return -1;
}
