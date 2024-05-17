uintptr_t Parcel::ipcData() const
{
 return reinterpret_cast<uintptr_t>(mData);
}
