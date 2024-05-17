static void addStringToSHA1(SHA1& sha1, const String& string)
{
    CString cString = string.utf8();
    sha1.addBytes(reinterpret_cast<const uint8_t*>(cString.data()), cString.length());
}
