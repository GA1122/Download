uintptr_t Parcel::readPointer() const
{
 return readAligned<binder_uintptr_t>();
}
