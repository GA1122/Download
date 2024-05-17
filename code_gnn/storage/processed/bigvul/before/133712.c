static String fullyDecodeString(const String& string, const WTF::TextEncoding& encoding)
{
    size_t oldWorkingStringLength;
    String workingString = string;
    do {
        oldWorkingStringLength = workingString.length();
        workingString = decode16BitUnicodeEscapeSequences(decodeStandardURLEscapeSequences(workingString, encoding));
    } while (workingString.length() < oldWorkingStringLength);
    workingString.replace('+', ' ');
    workingString = canonicalize(workingString);
    return workingString;
}
