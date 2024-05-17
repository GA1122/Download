uintptr_t Parcel::ipcObjects() const
{
 return reinterpret_cast<uintptr_t>(mObjects);
}
