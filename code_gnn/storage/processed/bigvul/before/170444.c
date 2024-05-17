status_t Parcel::growData(size_t len)
{
 size_t newSize = ((mDataSize+len)*3)/2;
 return (newSize <= mDataSize)
 ? (status_t) NO_MEMORY
 : continueWrite(newSize);
}
