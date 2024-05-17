double Parcel::readDouble() const
{
 union {
 double d;
 unsigned long long ll;
 } u;
    u.ll = readAligned<unsigned long long>();
 return u.d;
}
