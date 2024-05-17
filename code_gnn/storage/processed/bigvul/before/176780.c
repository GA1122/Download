status_t Parcel::writeByteVector(const std::unique_ptr<std::vector<uint8_t>>& val)
{
 return writeByteVectorInternalPtr(this, val);
}
