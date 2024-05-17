status_t Parcel::writeChar(char16_t val)
{
 return writeInt32(int32_t(val));
}
