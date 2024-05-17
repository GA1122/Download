status_t Parcel::readBool(bool *pArg) const
{
 int32_t tmp;
 status_t ret = readInt32(&tmp);
 *pArg = (tmp != 0);
 return ret;
}
