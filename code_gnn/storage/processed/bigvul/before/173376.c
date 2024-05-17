status_t Parcel::setDataCapacity(size_t size)
{
 if (size > INT32_MAX) {
 return BAD_VALUE;
 }

 if (size > mDataCapacity) return continueWrite(size);
 return NO_ERROR;
}
