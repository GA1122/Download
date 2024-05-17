static inline unsigned firstLetterLength(const String& text)
{
    unsigned length = 0;
    unsigned textLength = text.length();

    while (length < textLength && isSpaceForFirstLetter(text[length]))
        length++;

    while (length < textLength && isPunctuationForFirstLetter(text[length]))
        length++;

    if (isSpaceForFirstLetter(text[length]) || (textLength && length == textLength))
        return 0;

    length++;

    for (unsigned scanLength = length; scanLength < textLength; ++scanLength) {
        UChar c = text[scanLength];

        if (!isPunctuationForFirstLetter(c))
            break;

        length = scanLength + 1;
    }

    return length;
}
