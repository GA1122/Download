status_t Parcel::writeCharVector(const std::vector<char16_t>& val)
{
 return writeTypedVector(val, &Parcel::writeChar);
}
