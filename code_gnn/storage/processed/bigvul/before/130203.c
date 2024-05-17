bool base64Decode(const String& in, Vector<char>& out, CharacterMatchFunctionPtr shouldIgnoreCharacter, Base64DecodePolicy policy)
{
    if (in.isEmpty())
        return base64DecodeInternal<LChar>(0, 0, out, shouldIgnoreCharacter, policy);
    if (in.is8Bit())
        return base64DecodeInternal<LChar>(in.characters8(), in.length(), out, shouldIgnoreCharacter, policy);
    return base64DecodeInternal<UChar>(in.characters16(), in.length(), out, shouldIgnoreCharacter, policy);
}
