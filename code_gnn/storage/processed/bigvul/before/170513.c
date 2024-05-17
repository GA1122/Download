status_t Parcel::writeInt32(int32_t val)
{
 return writeAligned(val);
}
