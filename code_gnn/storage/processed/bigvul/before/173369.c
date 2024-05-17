uint64_t Parcel::readUint64() const
{
 return readAligned<uint64_t>();
}
