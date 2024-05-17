status_t Parcel::readByteVector(std::unique_ptr<std::vector<uint8_t>>* val) const {
 return readByteVectorInternalPtr(this, val);
}
