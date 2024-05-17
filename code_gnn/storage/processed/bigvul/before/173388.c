status_t Parcel::writeUint32(uint32_t val)
{
 return writeAligned(val);
}
