bool base64Decode(const char* data, unsigned length, Vector<char>& out, CharacterMatchFunctionPtr shouldIgnoreCharacter, Base64DecodePolicy policy)
{
    return base64DecodeInternal<LChar>(reinterpret_cast<const LChar*>(data), length, out, shouldIgnoreCharacter, policy);
}
