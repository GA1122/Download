size_t String8::getUtf32Length() const
{
 return utf8_to_utf32_length(mString, length());
}
