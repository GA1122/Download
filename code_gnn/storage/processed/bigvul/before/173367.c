status_t Parcel::readUint32(uint32_t *pArg) const
{
 return readAligned(pArg);
}
