static inline String decodeStandardURLEscapeSequences(const String& string, const WTF::TextEncoding& encoding)
{
    return decodeEscapeSequences<URLEscapeSequence>(string, encoding);
}
