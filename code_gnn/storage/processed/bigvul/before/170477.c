intptr_t Parcel::readIntPtr() const
{
 return readAligned<intptr_t>();
}
