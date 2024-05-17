static inline unsigned countGraphemesInCluster(const UChar* normalizedBuffer, unsigned normalizedBufferLength, uint16_t startIndex, uint16_t endIndex)
{
    if (startIndex > endIndex) {
        uint16_t tempIndex = startIndex;
        startIndex = endIndex;
        endIndex = tempIndex;
    }
    uint16_t length = endIndex - startIndex;
    ASSERT(static_cast<unsigned>(startIndex + length) <= normalizedBufferLength);
    TextBreakIterator* cursorPosIterator = cursorMovementIterator(&normalizedBuffer[startIndex], length);

    int cursorPos = cursorPosIterator->current();
    int numGraphemes = -1;
    while (0 <= cursorPos) {
        cursorPos = cursorPosIterator->next();
        numGraphemes++;
    }
    return numGraphemes < 0 ? 0 : numGraphemes;
}
