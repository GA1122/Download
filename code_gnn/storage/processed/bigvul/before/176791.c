status_t Parcel::writeInt32Vector(const std::vector<int32_t>& val)
{
 return writeTypedVector(val, &Parcel::writeInt32);
}
