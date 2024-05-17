status_t Parcel::writeDoubleVector(const std::unique_ptr<std::vector<double>>& val)
{
 return writeNullableTypedVector(val, &Parcel::writeDouble);
}
