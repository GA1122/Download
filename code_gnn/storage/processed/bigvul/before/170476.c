status_t Parcel::readIntPtr(intptr_t *pArg) const
{
 return readAligned(pArg);
}
