status_t Parcel::writeInt32Vector(const std::unique_ptr<std::vector<int32_t>>& val)
{
 return writeNullableTypedVector(val, &Parcel::writeInt32);
}
