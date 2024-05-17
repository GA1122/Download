size_t Parcel::dataSize() const
{
 return (mDataSize > mDataPos ? mDataSize : mDataPos);
}
