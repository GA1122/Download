status_t Parcel::setDataCapacity(size_t size)
{
 if (size > mDataCapacity) return continueWrite(size);
 return NO_ERROR;
}
