int8_t Parcel::readByte() const
{
 return int8_t(readInt32());
}
