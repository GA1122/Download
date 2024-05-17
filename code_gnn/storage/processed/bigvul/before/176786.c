status_t Parcel::writeDoubleVector(const std::vector<double>& val)
{
 return writeTypedVector(val, &Parcel::writeDouble);
}
