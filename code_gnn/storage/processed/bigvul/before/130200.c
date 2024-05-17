bool base64Decode(const Vector<char>& in, Vector<char>& out, CharacterMatchFunctionPtr shouldIgnoreCharacter, Base64DecodePolicy policy)
{
    out.clear();

    if (in.size() > UINT_MAX)
        return false;

    return base64Decode(in.data(), in.size(), out, shouldIgnoreCharacter, policy);
}
