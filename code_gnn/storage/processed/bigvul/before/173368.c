uint32_t Parcel::readUint32() const
{
 return readAligned<uint32_t>();
}
