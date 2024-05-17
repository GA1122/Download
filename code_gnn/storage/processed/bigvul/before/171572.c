void String8::getUtf32(char32_t* dst) const
{
    utf8_to_utf32(mString, length(), dst);
}
