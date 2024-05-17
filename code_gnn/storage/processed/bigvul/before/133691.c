static inline String decode16BitUnicodeEscapeSequences(const String& string)
{
    return decodeEscapeSequences<Unicode16BitEscapeSequence>(string, UTF8Encoding());
}
