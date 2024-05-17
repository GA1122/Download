status_t Parcel::writePointer(uintptr_t val)
{
 return writeAligned<binder_uintptr_t>(val);
}
