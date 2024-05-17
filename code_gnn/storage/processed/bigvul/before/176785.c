status_t Parcel::writeCharVector(const std::unique_ptr<std::vector<char16_t>>& val)
{
 return writeNullableTypedVector(val, &Parcel::writeChar);
}
