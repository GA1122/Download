status_t Parcel::readCharVector(std::unique_ptr<std::vector<char16_t>>* val) const {
 return readNullableTypedVector(val, &Parcel::readChar);
}
