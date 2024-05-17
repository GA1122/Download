int32_t String8::getUtf32At(size_t index, size_t *next_index) const
{
 return utf32_from_utf8_at(mString, length(), index, next_index);
}
