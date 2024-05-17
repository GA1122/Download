status_t Parcel::writeBool(bool val)
{
 return writeInt32(int32_t(val));
}
