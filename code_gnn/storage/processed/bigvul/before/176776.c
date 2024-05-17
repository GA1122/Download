status_t Parcel::writeBoolVector(const std::unique_ptr<std::vector<bool>>& val)
{
 return writeNullableTypedVector(val, &Parcel::writeBool);
}
