size_t String8::length() const
{
 return SharedBuffer::sizeFromData(mString)-1;
}
