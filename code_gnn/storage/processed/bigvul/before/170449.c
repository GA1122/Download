size_t Parcel::ipcDataSize() const
{
 return (mDataSize > mDataPos ? mDataSize : mDataPos);
}
