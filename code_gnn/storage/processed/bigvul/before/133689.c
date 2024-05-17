static String canonicalize(const String& string)
{
    return string.removeCharacters(&isNonCanonicalCharacter);
}
