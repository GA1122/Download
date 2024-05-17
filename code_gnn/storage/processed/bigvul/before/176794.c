status_t Parcel::writeInt64Vector(const std::unique_ptr<std::vector<int64_t>>& val)
{
 return writeNullableTypedVector(val, &Parcel::writeInt64);
}
