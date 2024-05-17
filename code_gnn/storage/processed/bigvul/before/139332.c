static const uint16_t* toUint16(const UChar* src)
{
    static_assert(sizeof(UChar) == sizeof(uint16_t), "UChar should be the same size as uint16_t");
    return reinterpret_cast<const uint16_t*>(src);
}
