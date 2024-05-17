status_t Parcel::writeFloatVector(const std::unique_ptr<std::vector<float>>& val)
{
 return writeNullableTypedVector(val, &Parcel::writeFloat);
}
