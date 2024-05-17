status_t Parcel::readByte(int8_t *pArg) const
{
 int32_t tmp;
 status_t ret = readInt32(&tmp);
 *pArg = int8_t(tmp);
 return ret;
}
