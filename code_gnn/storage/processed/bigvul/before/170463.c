status_t Parcel::readDouble(double *pArg) const
{
 union {
 double d;
 unsigned long long ll;
 } u;
    u.d = 0;
 status_t status;
    status = readAligned(&u.ll);
 *pArg = u.d;
 return status;
}
