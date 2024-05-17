status_t Parcel::writeString16Vector(const std::vector<String16>& val)
{
 return writeTypedVector(val, &Parcel::writeString16);
}
